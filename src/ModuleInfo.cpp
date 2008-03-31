#include "NastyCast.hpp"
#include "Modules/ModuleInfo.h"

ModuleInfo::ModuleInfo(IDynLib* module) : _module(module)
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

ModuleInfo::~ModuleInfo()
{
    this->destroy(this->_instance);
    delete _module;
}

int             ModuleInfo::getVersion() const
{
    return  this->version();
}
