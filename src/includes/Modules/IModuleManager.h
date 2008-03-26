#ifndef IMODULES_H
#define IMODULES_H

#include <iostream>

class   IModuleManager
{
    public:
        enum Support
        {
            ServerStart,
            PreReceive,
            PostReceive,
            PostBuild,
            PreContent,
            ProcessContent,
            ServerQuit
        };
        virtual ~IModuleManager() {};
        virtual bool    load(std::string filename) = 0;
};

#endif // IMODULES_H

