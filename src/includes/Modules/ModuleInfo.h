#ifndef MODULEINFO_UNIX_H
#define MODULEINFO_UNIX_H

#include <list>
#include <string>
#include "API/IModule.h"
#include "API/IModuleInfo.h"
#include "API/IDynLib.h"

class   ModuleInfo : public IModuleInfo
{
    public:
        ModuleInfo(IDynLib* module);
        ~ModuleInfo();
        IModule*                        getInstance() const;
        std::string                     getName() const;
    protected:
        create_t*                       create;
        destroy_t*                      destroy;
        name_t*                         name;
        IDynLib*                        _module;
        IModule*                        _instance;
        
};

#endif // MODULEINFO_UNIX_H

