#include "Config.h"

#include "MemoryManager.hpp"

Config::Config() : _globalConf(true)
{
	_params.clear();
}

Config::~Config()
{
	_params.clear();
}

Config::Config(const Config& right) : _mime(right._mime), _globalConf(false)
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

std::string		Config::getParam(std::string name) const
{
	//We have to go through with an iterator because operator[] isn't a const method.
	std::map<std::string, std::string>::const_iterator		it = this->_params.find(name);
	if (it == this->_params.end())
		return "";
	return it->second;
}

void			Config::setParam(std::string name, std::string value)
{
	this->_params[name] = value;
}
	
int		Config::getDefaultPort() const
{
	std::map<std::string, std::string>::const_iterator it = this->_params.find("UseSSL");

	if (it == this->_params.end()
			|| it->second == "1")
		return Config::_DefaultPort;
	return Config::_SslPort;
}

void	Config::addMimeType(std::string ext, std::string type)
{
	this->_mime[ext] = type;
}

std::string	Config::getMimeType(std::string ext) const
{
	std::map<std::string, std::string>::const_iterator	it = this->_mime.find(ext);

	if (it != this->_mime.end())
		return it->second;
	return "text/plain";
}

bool        Config::isSet(std::string name) const
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
