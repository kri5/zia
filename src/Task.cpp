#include "Task.h"
#include "IFile.h"
#include "File.h"
#include "HttpParser.h"
#include "HttpResponseFile.h"
#include "HttpResponseDir.h"
#include "MutexLock.hpp"
#include "IMutex.h"
#include "Mutex.h"
#include "Logger.hpp"
#include "RootConfig.hpp"
#include "Time.h"

#include "MemoryManager.hpp"


Task::Task(ClientSocket* clt, const std::vector<const Vhost*>& vhosts) :
    _res(NULL), _socket(clt), _time(NULL), _vhosts(vhosts)
{
    _req = new HttpRequest();
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

    if (this->_res)
        delete this->_res;
    if (this->_time)
        delete this->_time;
    //FIXME
    delete this->_socket;
    //selon le keep-alive : delete _socket;
}

void    Task::execute()
{
    _time = new Time();
    if (this->parseRequest() == true)
    {
        this->_time->init();
        if (this->buildResponse() == true)
        {
            //just for the moment :
            this->_res->appendOption("Server", "Ziahttp 0.2 (unix) Gentoo edition");
            this->_res->appendOption("Connection", "close");
            this->sendResponse();
        }
    }
}

bool    Task::parseRequest()
{
    HttpParser      parser(this->_req);
    char            tmp[1024];
    int             sockRet;
    char*           line;

    while (parser.done() == false)
    {
        if (this->checkTimeout())
            return false;
        sockRet = this->_socket->recv(tmp, 1024);
        if (sockRet < 0) //check recv timeout / error
        {
            if (errno == EAGAIN)
                continue ; //recv timeout.
            return false;
        }
        else if (sockRet == 0)
            return false; //connection closed.
        this->_readBuffer->push(tmp, sockRet);
        while (this->_readBuffer->hasEOL())
        {
            line = this->_readBuffer->getLine();
            parser.feed(line);
            delete[] line;
            this->_readBuffer->flush();
        }
        parser.parse();
    }
    //TODO: check host.
    this->_req->setConfig(Vhost::getVhost(this->_vhosts, 
                this->_req->getOption(HttpRequest::Host)));
    return true;
}

bool    Task::buildResponse()
{
    std::string     docRoot = this->_req->getConfig()->getParam("DocumentRoot");
    IFile*          fileInfo = new File(this->_req->getUri(), docRoot.c_str());

    if (fileInfo->getError() != IFile::Error::None)
    {
        if (fileInfo->getError() == IFile::Error::NoSuchFile)
            this->_res = new HttpError(404, this->_req);
        else if (fileInfo->getError() == IFile::Error::PermissionDenied)
            this->_res = new HttpError(403, this->_req);
        delete fileInfo;
        return true;
    }
    //TODO #43 : check if file is not found...
    if (fileInfo->isDirectory() == false)
    {
        this->_res = new HttpResponseFile(fileInfo);
        return true;
    }
    else
    {
        std::string     path(docRoot + this->_req->getUri());
        delete fileInfo;
        this->_res = new HttpResponseDir(this->_req);
        return true;
    }
}

bool    Task::sendResponse()
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
    if (this->sendBuffer() == false)
        return false;
    //From here, header is complete and send.
    std::iostream& stream = this->_res->getContent();
    char    buff[1024];
    do
    {
        stream.read(buff, sizeof(buff));
        this->_writeBuffer->push(buff, stream.gcount());
        if (this->sendBuffer() == false)
            return false;
    } while (this->_res->completed() == false);
    //this->_socket->close(true);
    return true;
}

bool        Task::sendBuffer()
{
    char*   line;
    int     ret;

    while (this->_writeBuffer->empty() == false)
    {
        line = this->_writeBuffer->get(1024);
        ret = this->_socket->send(line, this->_writeBuffer->gcount());
        if (ret == Socket::SOCKET_ERROR)
        {
            delete[] line;
            return false;
        }
        this->_writeBuffer->flush(ret);
        delete[] line;
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

