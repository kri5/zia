#include "Http/HttpOptionsCommand.h"

HttpOptionsCommand::HttpOptionsCommand()
{

}

HttpOptionsCommand::~HttpOptionsCommand()
{

}

void    HttpOptionsCommand::manageRequestBody(HttpRequest*, Buffer*, zAPI::IClientSocket*)
{

}

void    HttpOptionsCommand::execute(HttpRequest*, HttpResponse* res)
{
    res->setHeaderOption("Allow", "GET,HEAD,POST,OPTIONS");
    res->setHeaderOption("Content-Length", "0");
    res->setHeaderOption("Content-Type", "text/plain");
}
