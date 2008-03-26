#ifndef IMODULE_H
#define IMODULE_H

#include <string>

/// Every interface that will be used by modules must inherit from this one.
/// Here we set the module entry point, the destroy symbol and some
/// informations used to correctly register the module.
class   IModule
{
    public:
        virtual ~IModule() {};

        enum    ChainStatus
        {
            CONTINUE,
            BREAD,
            SKIP,
            STOP,
            ERROR
        };
};

typedef IModule*    create_t();
typedef void        destroy_t(IModule*);
typedef std::string name_t();

#endif // IMODULE_H
