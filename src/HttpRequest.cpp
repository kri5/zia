#include "HttpRequest.h"

#include <iostream>

HttpRequest::HttpRequest()
{

}

HttpRequest::~HttpRequest()
{

}

void            HttpRequest::setCommand(Command command)
{
    this->_command = command;
}

void            HttpRequest::setUri(std::string& uri)
{
    this->_uri = uri;
}

void            HttpRequest::appendOption(Option key,
                                          std::string& value)
{
    this->_options[key] = value;
}

void            HttpRequest::setProtocol(std::string& protocol)
{
    this->_protocol = protocol;
}

void            HttpRequest::print()
{
    std::map<Option, std::string>::iterator it
        = this->_options.begin();
    std::map<Option, std::string>::iterator ite
        = this->_options.end();

    std::cout << "----- HttpRequest -----" << std::endl
        << "Command == " << this->_command << std::endl
        << "Uri == " << this->_uri << std::endl
        << "Protocol == " << this->_protocol << std::endl
        << "-- Options --" << std::endl;;


    for (; it != ite; ++it)
        std::cout << "  option[" << (*it).first 
            << "] = " << (*it).second << std::endl;
    std::cout << "-- End Option --" << std::endl
        << "----- End HttpResponse -----" << std::endl;
}
