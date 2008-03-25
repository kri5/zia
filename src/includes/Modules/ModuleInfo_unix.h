#ifndef MODULEINFO_UNIX_H
#define MODULEINFO_UNIX_H

#include <list>
#include "IModule.h"
#include "IModules.h"
#include <dlfcn.h>

class   ModuleInfo
{
    public:
        ModuleInfo(void* handle);
        ~ModuleInfo();
        create_t*                       create;
        destroy_t*                      destroy;
        name_t*                         name;
        // This must be move to private fields later.
        std::list< IModules::Support >  support;

    protected:
        void*                           _handle;
        
};

#endif // MODULEINFO_UNIX_H

