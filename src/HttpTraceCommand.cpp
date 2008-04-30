#include "Http/HttpTraceCommand.h"
#include "Stream/ResponseStreamGeneric.h"

#include <sstream>
#include <map>
#include <iostream>

HttpTraceCommand::HttpTraceCommand()
{

}

HttpTraceCommand::~HttpTraceCommand()
{

}

void  HttpTraceCommand::manageRequestBody(HttpRequest*, Buffer*, zAPI::IClientSocket*)
{

}

void  HttpTraceCommand::execute(HttpRequest* req, HttpResponse* res)
{
    std::stringstream* content = new std::stringstream;

    std::map<std::string, std::string> headers = req->getHeaderOptions();

    std::map<std::string, std::string>::iterator    it = headers.begin();
    std::map<std::string, std::string>::iterator    ite = headers.end();

    *content << req->getCommand() << " ";
    if (req->getUri().empty())
        *content << "/";
    else
        *content << req->getUri();
    *content << " " << req->getProtocol() << "\r\n";
    for (; it != ite; ++it)
    {
        *content << (*it).first << ": " << (*it).second << "\r\n";
    }
    res->appendStream(new ResponseStreamGeneric(content));
    std::cout << "TRACE responseStream append to response" << std::endl;
}
