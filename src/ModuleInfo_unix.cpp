#include "API/ModuleInfo_unix.h"
#include <assert.h>

template<typename T, typename U>
U	nasty_cast(T symbol)
{
    assert(sizeof(T) == sizeof(U));
    union
    {
        T   symbol;
        U   function;
    }       cast;
    cast.symbol = symbol;
    return cast.function;
}


ModuleInfo::ModuleInfo(void* handle) : _handle(handle)
{
    name = nasty_cast<void*, name_t*>(dlsym(handle, "name"));
    destroy = nasty_cast<void*, destroy_t*>(dlsym(handle, "destroy"));
    create = nasty_cast<void*, create_t*>(dlsym(handle, "create"));
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

