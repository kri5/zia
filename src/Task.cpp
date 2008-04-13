#include <assert.h>
#include <errno.h>

#include "zia.h"

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
    if (RootConfig::isSet("Timeout"))
        _timeoutDelay = atoi(RootConfig::getParam("Timeout")->c_str());
    else
        _timeoutDelay = 300;//Arbitrary default value :o)

    poped = false;
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
}

void    Task::init(ClientSocket* clt,
        const std::vector<Vhost*>* vhosts)
{
    this->_vhosts = vhosts;
    this->_socket = clt;
    this->init();
    this->_freeTask = true;
}

void    Task::init()
{
    assert(poped == false);
    poped = true;
    this->_req = new HttpRequest();
    this->_res = new HttpResponse();
    this->_time = new Time();
}

void    Task::clear(bool clearBuffers)
{
    if (clearBuffers)
    {
        this->_writeBuffer->clear();
        this->_readBuffer->clear();
    }
    delete this->_time;
    delete this->_req;
    delete this->_res;
    poped = false;
}

void    Task::execute(unsigned int taskId)
{
    _taskId = taskId;
    this->_req->setRequestId(taskId);
    ModuleManager::getInstance().call(zAPI::IModule::WorkflowHook, zAPI::IModule::onBeginEvent, this->_req, this->_res);
    //std::cout << "new task" << std::endl;
    if (this->_socket->isClosed() == true)
    {
        this->finalize(false);
        return ;
    }
    if (this->_res->isInSendMode() == true || this->parseRequest() == true)
    {
        this->_time->init();
        if (this->_res->isInSendMode() == true || this->buildResponse() == true)
        {
            ModuleManager::getInstance().call(zAPI::IModule::BuildResponseHook, zAPI::IModule::onPostBuildEvent, this->_req, this->_res);
            //just for the moment :
            this->_res->setHeaderOption("Server", "Ziahttp 0.2 (unix) Gentoo edition");
            if (this->_req->headerOptionIsSet("Connection") && this->_req->getHeaderOption("Connection") == "close")
                this->_res->setHeaderOption("Connection", "close");
            else
                this->_res->setHeaderOption("Connection", "Keep-Alive");
            if (this->sendResponse())
            {
                this->finalize(true);
                return ;
            }
        }
    }
    ModuleManager::getInstance().call(zAPI::IModule::WorkflowHook, zAPI::IModule::onErrorEvent, this->_req, this->_res);
    this->finalize(false);
}

bool    Task::finalize(bool succeded)
{
    if (succeded)
        ModuleManager::getInstance().call(zAPI::IModule::WorkflowHook, zAPI::IModule::onEndEvent, this->_req, this->_res);

    if (succeded == false || this->_socket->isClosed() == true ||
            this->_req->headerOptionIsSet("Connection") && this->_req->getHeaderOption("Connection") == "close")
    {
        this->_freeTask = true;
        this->clear();
        delete this->_socket;
    }
    else
    {
        if (this->_readBuffer->empty() == false) //There are task left in this buffer.
        {
            this->_freeTask = false;
            this->clear(false);
            this->init(); //reinit task without touching internal buffers nor socket.
            this->_pool->rescheduleTask(this);
            return succeded;
        }
        else //free keep-alive client.
        {
            int         timeout;

            if (this->_req->headerOptionIsSet("Keep-Alive"))
                timeout = atoi(this->_req->getHeaderOption("Keep-Alive").c_str());
            else
                timeout = 150;
            this->_freeTask = true;
            this->clear();
            this->_pool->addKeepAliveClient(this->_socket, timeout, this->_vhosts);
        }
    }
    return succeded;
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
	{
#ifndef WIN32     
		Logger::getInstance() << "Poll error: " << strerror(errno) << Logger::Flush;
#else
		Logger::getInstance() << "Poll error: " << WSAGetLastError() << Logger::Flush;
#endif
	}
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

    ModuleManager::getInstance().call(zAPI::IModule::ReceiveRequestHook, zAPI::IModule::onPreReceiveEvent, this->_req, this->_res);
    while (parser.done() == false)
    {
        if (this->checkTimeout())
        {
            //this->_readBuffer->dump();
            exit(0);
            return false;
        }
        if (this->receiveDatas() == false)
            return false;
        parser.parse();
    }
    //FIXME: check host.
    this->_req->setConfig(Vhost::getVhost((*this->_vhosts), 
                this->_req->getHeaderOption("Host")));
    ModuleManager::getInstance().call(zAPI::IModule::ReceiveRequestHook, zAPI::IModule::onPostReceiveEvent, this->_req, this->_res);
    return true;
}

bool    Task::buildResponse()
{
    ModuleManager::getInstance().call(zAPI::IModule::BuildResponseHook, zAPI::IModule::onPreBuildEvent, this->_req, this->_res);
    const std::string&  docRoot = *(this->_req->getConfig()->getParam("DocumentRoot"));
    IFile*              fileInfo = new File(this->_req->getUri(), docRoot.c_str());

    //this->_readBuffer->dump();
    //std::cout << "===========================\n===============================" << std::endl;
    if (fileInfo->getError() != IFile::Error::None)
    {
        if (fileInfo->getError() == IFile::Error::NoSuchFile)
            this->_res->setError(new ErrorResponseStream(404, this->_req));
        else if (fileInfo->getError() == IFile::Error::PermissionDenied)
            this->_res->setError(new ErrorResponseStream(403, this->_req));
        //FIXME: add no more file descriptors as a potential error.
        delete fileInfo;
        return true;
    }
    if (fileInfo->isDirectory() == false)
    {
        this->_res->setHeaderOption("MimeType", 
              RootConfig::getInstance().getConfig()->getMimeType(fileInfo->getExtension()));
        this->_res->appendStream(new ResponseStreamFile(fileInfo));
    }
    else
    {
        std::string     path(docRoot + this->_req->getUri());
        delete fileInfo;
        zAPI::IResponseStream* stream = new ResponseStreamDir(this->_req);
        if (stream->good() == false)
        {
            delete stream;
            this->_res->setError(new ErrorResponseStream(500, this->_req));
            return true;
        }
        this->_res->setHeaderOption("Content-Type", "text/html");
        this->_res->appendStream(stream);
    }
    this->_res->setHeaderOption("Content-Length", this->_res->getContentLength());
    return true;
}

bool    Task::sendHeader()
{
    std::ostringstream      header;

    header << "HTTP/1.1 " << this->_res->getResponseStatus() << " " << this->_res->getResponseValue() << "\r\n";

    const std::map<std::string, std::string>& headerParams = this->_res->getHeaderOptions();
    std::map<std::string, std::string>::const_iterator        it = headerParams.begin();
    std::map<std::string, std::string>::const_iterator        ite = headerParams.end();
    while (it != ite)
    {
        header << it->first << ": " << it->second << "\r\n";
        ++it;
    }
	if (this->_res->headerInStream() == false)
	    header << "\r\n";
    const std::string& str = header.str();
    //this->_writeBuffer->clear();
    this->_writeBuffer->push(str.c_str(), str.length());
    return (this->sendBuffer());
}

bool    Task::sendResponse()
{
    ModuleManager::getInstance().call(zAPI::IModule::SendResponseHook, zAPI::IModule::onPreSendEvent, this->_req, this->_res);
    if (this->sendHeader() == false)
        return false;

    std::queue<zAPI::IResponseStream*>& streamQueue = this->_res->getStreams();
    char    buff[1024];
    size_t  size;
    zAPI::IResponseStream*    respStream;

    while (streamQueue.empty() == false)
    {
        respStream = streamQueue.front();
        std::iostream& stream = respStream->getContent();
        this->_res->setCurrentStream(&stream);
        streamQueue.pop();
        do
        {
            size = ModuleManager::getInstance().processContent(this->_req, this->_res, buff, 1024);
            this->_writeBuffer->push(buff, size);
            if (this->sendBuffer() == false)
                return false;
        } while (size == 1024);
        delete respStream;
    }
    //this->_socket->close(true);
    ModuleManager::getInstance().call(zAPI::IModule::SendResponseHook, zAPI::IModule::onPostSendEvent, this->_req, this->_res);
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

bool    Task::isFree() const
{
    return this->_freeTask;
}

