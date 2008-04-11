#include "Http/HttpRequest.h"

#include <iostream>
#include <iomanip>

HttpRequest::HttpRequest() : _relativeUri(true)
{
}

HttpRequest::~HttpRequest()
{
    this->_params.clear();
}

void            HttpRequest::setCommand(const std::string& command)
{
    this->_command = command;
}

void            HttpRequest::setUri(const std::string& uri, bool relative)
{
    this->_uri = uri;
    this->_relativeUri = relative;
    if (!this->_relativeUri)
        this->addDefaultHost();
}

void            HttpRequest::addDefaultHost()
{
   ; 
}

void			HttpRequest::setConfig(zAPI::IConfig* cfg)
{
	this->_cfg = cfg;
}

zAPI::IConfig*	HttpRequest::getConfig() const
{
	return this->_cfg;
}

void            HttpRequest::setUriArgument(const std::string& key,
                                            const std::string& value)
{
    this->_uriArguments[key] = value;
}

void            HttpRequest::setBodyArgument(const std::string& key,
                                                const std::string& value)
{
    this->_bodyArguments[key] = value;
}

const std::map<std::string, std::string>&   HttpRequest::getBodyArguments() const
{
    return this->_bodyArguments;
}

const std::string&      HttpRequest::getBodyArgument(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator    toFind = this->_bodyArguments.find(key);

    if (toFind == this->_bodyArguments.end())
    {
        const std::string& tmp = "";
        return (tmp);
    }
    return toFind->second;
}

const std::string&     HttpRequest::getCommand() const
{
    return this->_command;
}

const std::string&                          HttpRequest::getUri() const
{
    return this->_uri;
}

bool            HttpRequest::isUriRelative() const
{
    return this->_relativeUri;
}

const std::map<std::string, std::string>&   HttpRequest::getUriArguments() const
{
    return this->_uriArguments;
}

void            HttpRequest::clear()
{
    this->_options.clear();
    this->_uriArguments.clear();
    this->_bodyArguments.clear();

    this->_uri.clear();
    this->_protocol.clear();
    this->_relativeUri = false;
}

bool            HttpRequest::setParam(const std::string& key, void* value)
{
    if (this->_params.find(key) != this->_params.end())
        return false;
    this->_params[key] = value;
    return true;
}

void*           HttpRequest::getParam(const std::string& key) const
{
    std::map<std::string, void*>::const_iterator  value = this->_params.find(key);

    if (value == this->_params.end())
        return NULL;
    return value->second;
}

unsigned int    HttpRequest::getRequestId() const
{
    return this->_reqId;
}

void            HttpRequest::setRequestId(unsigned int id)
{
    this->_reqId = id;
}

void            HttpRequest::print()
{
    std::map<std::string, std::string>::iterator it
        = this->_options.begin();
    std::map<std::string, std::string>::iterator ite
        = this->_options.end();

    std::map<std::string, std::string>::iterator uriIt
        = this->_uriArguments.begin();
    std::map<std::string, std::string>::iterator uriIte
        = this->_uriArguments.end();

    std::cout << "----- HttpRequest -----" << std::endl
        << "Command == " << this->_command << std::endl
        << "Uri == " << this->_uri << std::endl
        << "UriRelative == " << std::boolalpha << this->_relativeUri << std::endl 
        << " =[UriArguments]=" << std::endl;

    for (; uriIt != uriIte; ++uriIt)
        std::cout << "  arg[" << (*uriIt).first
            << "] == " << (*uriIt).second << std::endl;



    std::cout << "Protocol == " << this->_protocol << std::endl
        << "-- Options --" << std::endl;;


    for (; it != ite; ++it)
        std::cout << "  option[" << (*it).first 
            << "] = " << (*it).second << std::endl;
    std::cout << "-- End Option --" << std::endl;

    uriIt = this->_bodyArguments.begin();
    uriIte = this->_bodyArguments.end();

    std::cout << "---BodyArguments----" << std::endl;
    for (; uriIt != uriIte; ++uriIt)
        std::cout << "  option[" << (*uriIt).first
            << "] = " << (*uriIt).second << std::endl;

    std::cout << "---EndBodyArgts----" << std::endl
        << "----- End HttpResponse -----" << std::endl;
}

