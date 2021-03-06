#include <iostream>
#include <cstdlib>

#include "Config.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

Config::Config() : _globalConf(true)
{
	_params.clear();
	_mime = new std::map<std::string, std::string>;
	_modules = new std::list<std::string>;
}

Config::~Config()
{
	if (this->_mime != NULL)
		delete this->_mime;
	if (this->_modules != NULL)
		delete this->_modules;
	_params.clear();
}

Config::Config(const Config& right) : IConfig(), _mime(NULL), _modules(NULL), _globalConf(false)
{
    //some parameters shouldn't be herited :
    std::map<std::string, std::string>::const_iterator    it = right._params.begin();
    std::map<std::string, std::string>::const_iterator    ite = right._params.end();

    while (it != ite)
    {
        if (it->first != "ServerName"
                && it->first != "Listen"
                && it->first != "LogLevel"
                && it->first != "UserDir"
                && it->first != "ErrorLog") //to be continued
            _params[it->first] = it->second;
        ++it;
    }
}

const std::string*      Config::getParam(const std::string& name) const
{
	//We have to go through with an iterator because operator[] isn't a const method.
	std::map<std::string, std::string>::const_iterator		it = this->_params.find(name);
	if (it == this->_params.end())
		return (NULL);
	return &(it->second);
}

const char*             Config::getParamChar(const std::string& name) const
{
	//We have to go through with an iterator because operator[] isn't a const method.
	std::map<std::string, std::string>::const_iterator		it = this->_params.find(name);
	if (it == this->_params.end())
		return (NULL);
	return it->second.c_str();
}

void			Config::setParam(const std::string& name, const std::string& value)
{
	this->_params[name] = value;
}

void	Config::addMimeType(std::string ext, std::string type)
{
	(*this->_mime)[ext] = type;
}

std::string	Config::getMimeType(std::string ext) const
{
	if (this->_mime == NULL)
		throw ZException<Config>(INFO, Error::NotRootConfig);
	std::map<std::string, std::string>::const_iterator	it = this->_mime->find(ext);

	if (it != this->_mime->end())
		return it->second;
	return "text/plain";
}

void	Config::addModule(std::string location)
{
    Logger::getInstance() << Logger::Info << "Adding module \"" << location << '"' << Logger::Flush;
	this->_modules->push_back(location);
}

const std::list<std::string>&	Config::getModules() const
{
	if (this->_modules == NULL)
		throw ZException<Config>(INFO, Error::NotRootConfig);
    return *(this->_modules);
}

bool        Config::isSet(const std::string& name) const
{
    return (this->_params.find(name) != this->_params.end());
}

void        Config::removeParameter(std::string name)
{
    this->_params.erase(name);
}

void        Config::dump() const
{
    std::map<std::string, std::string>::const_iterator it = this->_params.begin();
    std::map<std::string, std::string>::const_iterator ite = this->_params.end();

    while (it != ite)
    {
        std::cout << it->first << " => " << it->second << std::endl;
        ++it;
    }
}

int         Config::getIntParam(const std::string& key, int defaultValue) const
{
    int nb;

    if (this->isSet(key))
    {
        std::string threadNb = *this->getParam(key);
        if ((nb = atoi(threadNb.c_str())) != 0)
            return nb;
    }
    return defaultValue;
}
