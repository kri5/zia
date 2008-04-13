#ifndef IMODULEINFO_H__
# define IMODULEINFO_H__

#include <vector>

#include "IModule.h"

namespace zAPI
{
    /// This class will contain base symbols, and the module instance.
    class   IModuleInfo
    {
        public:
            virtual                                                 ~IModuleInfo(){}
            /// Will return the module instance.
            virtual zAPI::IModule*                                  getInstance() const = 0;
            /// Will return the module name.
            virtual const std::string&                              getName() const = 0;
            /// Will return the module version.
            virtual float                                           getVersion() const = 0;
            virtual const std::string&                              getFileName() const = 0;

            virtual void                                            addSupportedHook(IModule::Hook) = 0;
            virtual const std::vector<zAPI::IModule::Hook>&         getSupportedHooks() const = 0;
    };
}

#endif //IMODULEINFO_H__

