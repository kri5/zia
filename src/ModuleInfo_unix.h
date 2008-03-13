#ifndef MODULEINFO_UNIX_H
#define MODULEINFO_UNIX_H

#include "IModule.h"
#include <dlfcn.h>

class   ModuleInfo
{
    public:
        ModuleInfo(void* handle);
        ~ModuleInfo();
        create_t*           create;
        destroy_t*          destroy;
        name_t*             name;

    protected:
        void*               _handle;
};

#endif // MODULEINFO_UNIX_H

