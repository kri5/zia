#include "Http/HttpPostCommand.h"
#include "Http/HttpGetCommand.h"
#include "File/IFile.h"
#include "File/File.h"
#include "RootConfig.hpp"
#include "Stream/ResponseStreamDir.h"
#include "Stream/ResponseStreamFile.h"
#include "Stream/ResponseStreamSocket.h"
#include "API/IModule.h"
#include "API/IBuildResponse.h"

HttpPostCommand::HttpPostCommand()
{

}

HttpPostCommand::~HttpPostCommand()
{

}

void        HttpPostCommand::manageRequestBody(HttpRequest* req, Buffer* readBuffer, zAPI::IClientSocket* socket)
{
    std::string     buff;
    readBuffer->get(buff, readBuffer->size());
    req->setBodyStream(new ResponseStreamSocket(socket, buff, readBuffer->size()));
    readBuffer->flush();
}

void        HttpPostCommand::execute(HttpRequest* req, HttpResponse* res)
{
    const std::string&  docRoot = *(req->getConfig()->getParam("DocumentRoot"));
    IFile*              fileInfo = new File(req->getUri(), docRoot.c_str());

    if (fileInfo->getError() != IFile::Error::None)
    {
        if (fileInfo->getError() == IFile::Error::NoSuchFile)
            res->setError(new ErrorResponseStream(404, req));
        else if (fileInfo->getError() == IFile::Error::PermissionDenied)
            res->setError(new ErrorResponseStream(403, req));
        //FIXME: add no more file descriptors as a potential error.
        delete fileInfo;
        return ; 
    }
    if (fileInfo->isDirectory() == false)
    {
        res->setHeaderOption("MimeType", 
              RootConfig::getInstance().getConfig()->getMimeType(fileInfo->getExtension()));
        res->appendStream(new ResponseStreamFile(fileInfo));
    }
    else
    {
        std::string     path(docRoot + req->getUri());
        delete fileInfo;
        zAPI::IResponseStream* stream = new ResponseStreamDir(req);
        if (stream->good() == false)
        {
            delete stream;
            res->setError(new ErrorResponseStream(500, req));
            return ;
        }
	    if (res->getHeaderInStream() == false)
            res->setHeaderOption("Content-Type", "text/html");
        res->appendStream(stream);
    }
	if (res->getHeaderInStream() == false)
        res->setHeaderOption("Content-Length", res->getContentLength());
}
