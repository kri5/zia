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
        IModule*                        getInstance();
    protected:
        create_t*                       create;
        destroy_t*                      destroy;
        name_t*                         name;
        void*                           _handle;
        IModule*                        _instance;
        
};

#endif // MODULEINFO_UNIX_H

