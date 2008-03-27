#include <errno.h>

#include "Workflow/Pool.h" //Let this in first position (crossed dependencies :/ )
#include "Workflow/Task.h"
#include "File/IFile.h"
#include "File/File.h"
#include "Http/HttpParser.h"
#include "MutexLock.hpp"
#include "Mutex/IMutex.h"
#include "Mutex/Mutex.h"
#include "Logger.hpp"
#include "RootConfig.hpp"
#include "Time/Time.h"
#include "Stream/ResponseStreamDir.h"
#include "Stream/ResponseStreamFile.h"
#include "Stream/ErrorResponseStream.h" //FIXME : change the name incoherence...
#include "Modules/ModuleManager.h"

#include "MemoryManager.hpp"


Task::Task(Pool* pool) :
    _res(NULL), _socket(NULL), _time(NULL), _pool(pool), _vhosts(NULL)
{
    _readBuffer = new Buffer(1024);
    _writeBuffer = new Buffer(1024);
    _timeoutDelay = atoi(RootConfig::getParam("Timeout").c_str());
}

Task::~Task()
{
    this->_writeBuffer->clear();
    delete this->_writeBuffer;

    this->_readBuffer->clear();
    delete this->_readBuffer;

    delete this->_req;
    delete this->_res;

    if (this->_time)
        delete this->_time;
    //FIXME
    delete this->_socket;
    //selon le keep-alive : delete _socket;
}

void    Task::init(ClientSocket* clt,
        const std::vector<const Vhost*>* vhosts)
{
    this->_vhosts = vhosts;
    this->_socket = clt;
    this->init();
}

void    Task::init()
{
    this->_req = new HttpRequest();
    this->_res = new HttpResponse();
}

void    Task::clear(bool clearBuffers)
{
    if (clearBuffers)
    {
        this->_writeBuffer->clear();
        this->_readBuffer->clear();
    }
    delete this->_req;
    delete this->_res;
}

bool    Task::finalize(bool succeded)
{
//    if (succeded == false ||
//            this->_req->optionIsSet("Connection") && this->_req->getOption("Connection") == "close")
//    {
        delete this->_socket;
//    }
//    else
//    {
//        std::cout << "adding keep alive to queue" << std::endl;
//        if (this->_readBuffer->empty() == false)
//        {
//            this->clear(false);
//            this->_pool->rescheduleTask(this);
//            return succeded; //do not clear the task buffers !
//        }
//        else
//            this->_pool->addKeepAliveClient(this->_socket, this->_vhosts);
//    }
    this->clear();
    //std::cout << "end task" << std::endl;
    return succeded;
}

void    Task::execute()
{
    _time = new Time();
    //std::cout << "new task" << std::endl;
    if (this->parseRequest() == true)
    {
        this->_time->init();
        if (this->buildResponse() == true)
        {
            //just for the moment :
            this->_res->appendOption("Server", "Ziahttp 0.2 (unix) Gentoo edition");
            if (this->_req->optionIsSet("Connection") && this->_req->getOption("Connection") == "close")
                this->_res->appendOption("Connection", "close");
            else
            {
                this->_res->appendOption("Connection", "Keep-Alive");
            }
            if (this->sendResponse())
            {
                this->finalize(true);
                return ;
            }
        }
    }
    this->finalize(false);
}

bool    Task::receiveDatas()
{
    struct pollfd   fds;
    int             ret;
    int             sockRet;
    char            tmp[1025];

    memset(&fds, 0, sizeof(fds));
    *(this->_socket) >> fds;
    ret = poll(&fds, 1, 1);
    if (ret < 0)
        Logger::getInstance() << "Poll error: " << strerror(errno) << Logger::Flush;
    if (ret > 0)
    {
        sockRet = this->_socket->recv(tmp, 1024);
        tmp[sockRet] = 0;
        //std::cout << "readed [" << tmp << "]" << std::endl;
        if (sockRet <= 0) //check recv / error
            return false;
        this->_readBuffer->push(tmp, sockRet);
    }
    return true;
}

bool    Task::parseRequest()
{
    HttpParser      parser(this->_req, this->_readBuffer);

    while (parser.done() == false)
    {
        if (this->checkTimeout())
        {
            //std::cout << "PRE TIMEOUT" << std::endl;
            this->_readBuffer->dump();
            return false;
        }
        this->receiveDatas();
        parser.parse();
    }
    //TODO: check host.
    this->_req->setConfig(Vhost::getVhost((*this->_vhosts), 
                this->_req->getOption("Host")));
    return true;
}

bool    Task::buildResponse()
{
    std::string     docRoot = this->_req->getConfig()->getParam("DocumentRoot");
    IFile*          fileInfo = new File(this->_req->getUri(), docRoot.c_str());

    if (fileInfo->getError() != IFile::Error::None)
    {
        if (fileInfo->getError() == IFile::Error::NoSuchFile)
            this->_res->setError(new ErrorResponseStream(404, this->_req));
        else if (fileInfo->getError() == IFile::Error::PermissionDenied)
            this->_res->setError(new ErrorResponseStream(403, this->_req));
        delete fileInfo;
        return true;
    }
    //TODO #43 : check if file is not found...
    if (fileInfo->isDirectory() == false)
    {
        this->_res->appendOption("MimeType", 
              RootConfig::getInstance().getConfig()->getMimeType(fileInfo->getExtension()));
        this->_res->appendStream(new ResponseStreamFile(fileInfo));
    }
    else
    {
        std::string     path(docRoot + this->_req->getUri());
        delete fileInfo;
        this->_res->appendStream(new ResponseStreamDir(this->_req));
    }
    this->_res->appendOption("Content-Length", this->_res->getContentLength());
    return true;
}

bool    Task::sendHeader()
{
    std::ostringstream      header;

    header << "HTTP/1.1 " << this->_res->getResponseStatus() << " " << this->_res->getResponseValue() << "\r\n";

    const std::map<std::string, std::string>& headerParams = this->_res->getOptions();
    std::map<std::string, std::string>::const_iterator        it = headerParams.begin();
    std::map<std::string, std::string>::const_iterator        ite = headerParams.end();
    while (it != ite)
    {
        header << it->first << ": " << it->second << "\r\n";
        ++it;
    }
    header << "\r\n";
    const std::string& str = header.str();
    //this->_writeBuffer->clear();
    this->_writeBuffer->push(str.c_str(), str.length());
    return (this->sendBuffer());
}

bool    Task::sendResponse()
{
    ModuleManager::getInstance()->call(IModuleManager::SendResponseHook, IModule::onPreSendEvent, this->_req, this->_res);
    if (this->sendHeader() == false)
        return false;

    std::queue<IResponseStream*>& streamQueue = this->_res->getStreams();
    char    buff[1024];
    size_t  size;
    IResponseStream*    respStream;

    while (streamQueue.empty() == false)
    {
        respStream = streamQueue.front();
        std::iostream& stream = respStream->getContent();
        this->_res->setCurrentContent(&stream);
        streamQueue.pop();
        do
        {
            size = ModuleManager::getInstance()->processContent(this->_req, this->_res, buff, 1024);
            this->_writeBuffer->push(buff, size);
            if (this->sendBuffer() == false)
                return false;
        } while (size == 1024);
        delete respStream;
    }
    //this->_socket->close(true);
    ModuleManager::getInstance()->call(IModuleManager::SendResponseHook, IModule::onPostSendEvent, this->_req, this->_res);
    return true;
}

bool        Task::sendBuffer()
{
    char	line[1025];
    int     ret;

    while (this->_writeBuffer->empty() == false)
    {
        this->_writeBuffer->get(line, 1024);
        ret = this->_socket->send(line, this->_writeBuffer->gcount());
        if (ret < 0)
            return false;
        this->_writeBuffer->flush(ret);
    }
    this->_writeBuffer->clear();
    return true;
}

bool    Task::checkTimeout()
{
    if (this->_time->elapsed(this->_timeoutDelay))
    {
        Logger::getInstance() << Logger::Info << "Client has timeout" << Logger::Flush;
        return true;
    }
    return false;
}

