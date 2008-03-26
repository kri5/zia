#ifndef IMODULES_H
#define IMODULES_H

#include <iostream>

class   IModuleManager
{
    public:
        enum    Hook
        {
            ServerEventHook,
            ModuleEventHook,
            WorkflowHook,
            NetworkHook,
            ReceiveRequestHook,
            BuildResponseHook,
            SendResponseHook,
            NumberOfHooks
        };
        virtual ~IModuleManager() {};
        virtual bool    load(std::string filename) = 0;
};

#endif // IMODULES_H

