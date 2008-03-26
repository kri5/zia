#include "API/ModuleInfo_unix.h"

ModuleInfo::ModuleInfo(void* handle) : _handle(handle)
{
    name = (name_t*) dlsym(handle, "name");
    destroy = (destroy_t*) dlsym(handle, "destroy");
    create = (create_t*) dlsym(handle, "create");
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
    dlclose(_handle);
}

