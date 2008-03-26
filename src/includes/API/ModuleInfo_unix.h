#ifndef MODULEINFO_UNIX_H
#define MODULEINFO_UNIX_H

#include <list>
#include <string>
#include "API/IModule.h"
#include <dlfcn.h>

class   ModuleInfo
{
    public:
        ModuleInfo(void* handle);
        ~ModuleInfo();
        IModule*                        getInstance() const;
        std::string                     getName() const;
    protected:
        create_t*                       create;
        destroy_t*                      destroy;
        name_t*                         name;
        void*                           _handle;
        IModule*                        _instance;
        
};

#endif // MODULEINFO_UNIX_H

