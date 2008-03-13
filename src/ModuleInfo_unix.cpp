#include "ModuleInfo_unix.h"

ModuleInfo::ModuleInfo(void* handle) : _handle(handle)
{
    name = (name_t*) dlsym(handle, "name");
    destroy = (destroy_t*) dlsym(handle, "destroy");
    create = (create_t*) dlsym(handle, "create");
}

ModuleInfo::~ModuleInfo()
{
    dlclose(_handle);
}
