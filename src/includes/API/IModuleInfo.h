#ifndef IMODULEINFO_H__
# define IMODULEINFO_H__

/// Interface for ModuleInfo.
/// This class will contain base symbols, and the module instance.
class   IModuleInfo
{
    public:
        virtual             ~IModuleInfo(){}
        /// Will return the module instance.
        virtual IModule*    getInstance() const = 0;
        /// Will return the module name.
        virtual std::string getName() const = 0;
        /// Will return the module version.
        virtual int         getVersion() const = 0;
};

#endif //IMODULEINFO_H__

