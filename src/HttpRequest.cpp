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

void            HttpRequest::appendUriArgument(std::string& key,
                                            std::string& value)
{
    this->_uriArguments[key] = value;
}


void            HttpRequest::setProtocol(std::string& protocol)
{
    this->_protocol = protocol;
}

HttpRequest::Command                        HttpRequest::getCommand()
{
    return this->_command;
}

const std::string&                          HttpRequest::getUri()
{
    return this->_uri;
}

const std::string&                          HttpRequest::getProtocol()
{
    return this->_protocol;
}

const std::map<HttpRequest::Option, std::string>&        HttpRequest::getOptions()
{
    return this->_options;
}

const std::map<std::string, std::string>&   HttpRequest::getUriArguments()
{
    return this->_uriArguments;
}

void            HttpRequest::print()
{
    std::map<Option, std::string>::iterator it
        = this->_options.begin();
    std::map<Option, std::string>::iterator ite
        = this->_options.end();

    std::map<std::string, std::string>::iterator uriIt
        = this->_uriArguments.begin();
    std::map<std::string, std::string>::iterator uriIte
        = this->_uriArguments.end();


    std::cout << "----- HttpRequest -----" << std::endl
        << "Command == " << this->_command << std::endl
        << "Uri == " << this->_uri << std::endl
        << " =[UriArguments]=" << std::endl;

    for (; uriIt != uriIte; ++uriIt)
        std::cout << "  arg[" << (*uriIt).first
            << "] == " << (*uriIt).second << std::endl;



    std::cout << "Protocol == " << this->_protocol << std::endl
        << "-- Options --" << std::endl;;


    for (; it != ite; ++it)
        std::cout << "  option[" << (*it).first 
            << "] = " << (*it).second << std::endl;
    std::cout << "-- End Option --" << std::endl
        << "----- End HttpResponse -----" << std::endl;
}
