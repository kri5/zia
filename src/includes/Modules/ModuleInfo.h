#ifndef MODULEINFO_UNIX_H
#define MODULEINFO_UNIX_H

#include <list>
#include <string>
#include <vector>

#include "API/IModule.h"
#include "API/IModuleInfo.h"
#include "API/IDynLib.h"

class   ModuleInfo : public zAPI::IModuleInfo
{
    public:
        ModuleInfo(zAPI::IDynLib* module, const std::string&);
        ~ModuleInfo();
        zAPI::IModule*                              getInstance() const;
        std::string                                 getName() const;
        int                                         getVersion() const;
        const std::string&                          getFileName() const;
        void                                        addSupportedHook(zAPI::IModule::Hook);
        const std::vector<zAPI::IModule::Hook>&     getSupportedHooks() const;
    protected:
        zAPI::create_t*                                   create;
        zAPI::destroy_t*                                  destroy;
        zAPI::name_t*                                     name;
        zAPI::version_t*                                  version;
        zAPI::IDynLib*                              _module;
        zAPI::IModule*                              _instance;
        std::string                                 _fileName;
        std::vector<zAPI::IModule::Hook>            _hooks;
        
};

#endif // MODULEINFO_UNIX_H

