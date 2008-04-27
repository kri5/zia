#include "Http/HttpCommandBuilder.h"

#include "Http/HttpGetCommand.h"
#include "Http/HttpPostCommand.h"
#include "Http/HttpHeadCommand.h"
#include "Http/HttpErrorCommand.h"
#include "Http/HttpOptionsCommand.h"

HttpCommandBuilder::HttpCommandBuilder()
{

}

HttpCommandBuilder::~HttpCommandBuilder()
{

}

IHttpCommand*       HttpCommandBuilder::buildCommand(HttpRequest* req)
{
    if (req->isUriRelative()
        && !req->headerOptionIsSet("Host"))
        return new HttpErrorCommand(400);

    if (req->getCommand() == "GET")
        return new HttpGetCommand();
    else if (req->getCommand() == "POST")
        return new HttpPostCommand();
    else if (req->getCommand() == "HEAD")
        return new HttpHeadCommand();
    else if (req->getCommand() == "OPTIONS")
        return new HttpOptionsCommand();
    return new HttpErrorCommand(501);
}
