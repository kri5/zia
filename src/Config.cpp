#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <sstream>

#include "Config.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

Config::Config() : _globalConf(true) {
}

Config::~Config() {
}

Config::Config(const Config &right)
    : IConfig(), _globalConf(false) {
  std::vector<std::string> v {
    "ServerName", "Listen", "LogLevel", "UserDir", "ErrorLog"
  };

  for (auto param : right._params) {
    if (std::find(std::begin(v), std::end(v), param.first) != std::end(v))
      this->_params.insert(param);
  }
}

const std::string *Config::getParam(const std::string &name) const {
  //We have to go through with an iterator because operator[] isn't a const
  //method.
  auto it = this->_params.find(name);
  return (it == std::end(this->_params) ? nullptr : &(it->second));
}

const char *Config::getParamChar(const std::string &name) const {
  //We have to go through with an iterator because operator[] isn't a const
  //method.
  auto it = this->_params.find(name);
  return (it == std::end(this->_params) ? nullptr : it->second.c_str());
}

void Config::setParam(const std::string &name, const std::string &value) {
  this->_params[name] = value;
}

void Config::addMimeType(std::string ext, std::string type) {
  this->_mime[ext] = type;
}

std::string Config::getMimeType(std::string ext) const {
  if (this->_mime.empty())
    throw ZException<Config>(INFO, Error::NotRootConfig);
  auto it = this->_mime.find(ext);
  return (it != std::end(this->_mime) ? it->second : "text/plain");
}

void Config::addModule(std::string location) {
  Logger::getInstance() << Logger::Info << "Adding module \"" << location << '"'
                        << Logger::Flush;
  this->_modules.push_back(location);
}

const std::list<std::string> &Config::getModules() const {
  if (this->_modules.empty())
    throw ZException<Config>(INFO, Error::NotRootConfig);
  return this->_modules;
}

bool Config::isSet(const std::string &name) const {
  return (this->_params.find(name) != std::end(this->_params));
}

void Config::removeParameter(std::string name) { this->_params.erase(name); }

void Config::dump() const {
  for (auto param : this->_params)
    std::cout << param.first << " => " << param.second << std::endl;
}

int Config::getIntParam(const std::string &key, int defaultValue) const {
  int nb = 0;

  if (this->isSet(key))
	  std::istringstream(*this->getParam(key)) >> nb;
  return nb != 0 ? nb : defaultValue;
}
