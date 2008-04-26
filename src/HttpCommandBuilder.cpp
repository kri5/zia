#include "Http/HttpCommandBuilder.h"

#include "Http/HttpGetCommand.h"
#include "Http/HttpPostCommand.h"
#include "Http/HttpHeadCommand.h"

HttpCommandBuilder::HttpCommandBuilder()
{

}

HttpCommandBuilder::~HttpCommandBuilder()
{

}

IHttpCommand*       HttpCommandBuilder::buildCommand(zAPI::IHttpRequest* req)
{
    if (req->getCommand() == "GET")
        return new HttpGetCommand();
    else if (req->getCommand() == "POST")
        return new HttpPostCommand();
    else if (req->getCommand() == "HEAD")
        return new HttpHeadCommand();
    return NULL;
}
