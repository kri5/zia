#include "Http/HttpRequest.h"

#include <iostream>
#include <iomanip>

HttpRequest::HttpRequest() : _relativeUri(true), _bodyStream(NULL), _clt(NULL)
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


const std::string&     HttpRequest::getUriQuery() const
{
    return this->_uriArguments;
}

void                    HttpRequest::setUriQuery(const std::string& params)
{
    this->_uriArguments = params;
}

void            HttpRequest::clear()
{
    this->_options.clear();

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

zAPI::IResponseStream*  HttpRequest::getBodyStream() const
{
    return this->_bodyStream;
}

void                    HttpRequest::setBodyStream(zAPI::IResponseStream* stream)
{
    this->_bodyStream = stream;
}

void                    HttpRequest::setClient(zAPI::IClientSocket* clt)
{
    _clt = clt;
}

zAPI::IClientSocket*    HttpRequest::getClient() const
{
    return this->_clt;
}

void            HttpRequest::print()
{
    std::map<std::string, std::string>::iterator it
        = this->_options.begin();
    std::map<std::string, std::string>::iterator ite
        = this->_options.end();

    std::cout << "----- HttpRequest -----" << std::endl
        << "Command == " << this->_command << std::endl
        << "Uri == " << this->_uri << std::endl
        << "UriRelative == " << std::boolalpha << this->_relativeUri << std::endl 
        << " =[UriArguments]=" << std::endl;

    std::cout << "Protocol == " << this->_protocol << std::endl
        << "-- Options --" << std::endl;;


    for (; it != ite; ++it)
        std::cout << "  option[" << (*it).first 
            << "] = " << (*it).second << std::endl;
    std::cout << "-- End Option --" << std::endl
        << "----- End HttpResponse -----" << std::endl;
}

