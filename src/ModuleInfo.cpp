#include "NastyCast.hpp"
#include "Modules/ModuleInfo.h"

ModuleInfo::ModuleInfo(IDynLib* module, const std::string& filename) : _module(module), _fileName(filename)
{
    name = nasty_cast<void*, name_t*>(module->sym("name"));
    destroy = nasty_cast<void*, destroy_t*>(module->sym("destroy"));
    create = nasty_cast<void*, create_t*>(module->sym("create"));
    version = nasty_cast<void*, version_t*>(module->sym("version"));
    _instance = create();
}

IModule*        ModuleInfo::getInstance() const
{
    return this->_instance;
}

std::string     ModuleInfo::getName() const
{
    return this->name();
}

const std::string&  ModuleInfo::getFileName() const
{
    return this->_fileName;
}

ModuleInfo::~ModuleInfo()
{
    this->destroy(this->_instance);
    delete _module;
}

int             ModuleInfo::getVersion() const
{
    return  this->version();
}

void            ModuleInfo::addSupportedHook(IModuleManager::Hook hook)
{
    this->_hooks.push_back(hook);
}

const std::vector<IModuleManager::Hook>&    ModuleInfo::getSupportedHooks() const
{
    return this->_hooks;
}
