#include "NastyCast.hpp"
#include "Modules/ModuleInfo.h"

ModuleInfo::ModuleInfo(IDynLib* module, const std::string& filename) : _module(module), _fileName(filename)
{
    name = nasty_cast<void*, zAPI::name_t*>(module->sym("name"));
    destroy = nasty_cast<void*, zAPI::destroy_t*>(module->sym("destroy"));
    create = nasty_cast<void*, zAPI::create_t*>(module->sym("create"));
    version = nasty_cast<void*, zAPI::version_t*>(module->sym("version"));
    _instance = create();
    _name = name();
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
    return  this->version();
}

void            ModuleInfo::addSupportedHook(zAPI::IModule::Hook hook)
{
    this->_hooks.push_back(hook);
}

const std::vector<zAPI::IModule::Hook>&    ModuleInfo::getSupportedHooks() const
{
    return this->_hooks;
}
