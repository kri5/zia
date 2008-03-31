#ifndef MODULEINFO_UNIX_H
#define MODULEINFO_UNIX_H

#include <list>
#include <string>
#include <vector>

#include "API/IModule.h"
#include "API/IModuleInfo.h"
#include "API/IDynLib.h"

class   ModuleInfo : public IModuleInfo
{
    public:
        ModuleInfo(IDynLib* module, const std::string&);
        ~ModuleInfo();
        IModule*                                    getInstance() const;
        std::string                                 getName() const;
        int                                         getVersion() const;
        const std::string&                          getFileName() const;
        void                                        addSupportedHook(IModuleManager::Hook);
        const std::vector<IModuleManager::Hook>&    getSupportedHooks() const;
    protected:
        create_t*                                   create;
        destroy_t*                                  destroy;
        name_t*                                     name;
        version_t*                                  version;
        IDynLib*                                    _module;
        IModule*                                    _instance;
        std::string                                 _fileName;
        std::vector<IModuleManager::Hook>           _hooks;
        
};

#endif // MODULEINFO_UNIX_H

