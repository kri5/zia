#include "Http/HttpErrorCommand.h"
#include "Stream/ErrorResponseStream.h"

HttpErrorCommand::HttpErrorCommand(int error) : _error(error)
{

}

HttpErrorCommand::~HttpErrorCommand()
{

}

void    HttpErrorCommand::manageRequestBody(HttpRequest*, Buffer*, zAPI::IClientSocket*)
{
    
}

void    HttpErrorCommand::execute(HttpRequest* req, HttpResponse* res)
{
    res->setError(new ErrorResponseStream(this->_error, req));
}
