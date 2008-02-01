#include "Config.h"

Config::Config()
{
	_params.clear();
}

Config::~Config()
{
	_params.clear();
}

Config::Config(const Config& right)
{
	_params = right._params;
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


/**********************************************************************************
**********************************************************************************
**********************************************************************************
**********************************************************************************
**********************************************************************************
**********************************************************************************
**********************************************************************************
**********************************************************************************/


RootConfig::RootConfig() : Config()
{
	_address.clear();
	_globalVhosts.clear();
}

RootConfig::~RootConfig()
{
	_address.clear();
	_globalVhosts.clear();
}

