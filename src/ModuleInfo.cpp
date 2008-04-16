#include "NastyCast.hpp"
#include "zia.h"
#include "Modules/ModuleInfo.h"
#include <iostream>

#define LOAD_SYMBOL(name) *(void **)(&name) = module->sym(#name)

ModuleInfo::ModuleInfo(IDynLib* module, const std::string& filename) : _module(module), _fileName(filename)
{
    LOAD_SYMBOL(name);
    LOAD_SYMBOL(destroy);
    LOAD_SYMBOL(create);
    LOAD_SYMBOL(version);
    _instance = (*create)();
    _name = (*name)();
}

zAPI::IModule*          ModuleInfo::getInstance() const
{
    return this->_instance;
}

const std::string&      ModuleInfo::getName() const
{
    return this->_name;
}

const std::string&      ModuleInfo::getFileName() const
{
    return this->_fileName;
}

ModuleInfo::~ModuleInfo()
{
    this->destroy(this->_instance);
    delete _module;
}

float           ModuleInfo::getVersion() const
{
    return this->version();
}

void            ModuleInfo::addSupportedHook(zAPI::IModule::Hook hook)
{
    this->_hooks.push_back(hook);
}

const std::vector<zAPI::IModule::Hook>&    ModuleInfo::getSupportedHooks() const
{
    return this->_hooks;
}
