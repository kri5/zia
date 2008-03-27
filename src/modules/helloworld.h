#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include "API/IModule.h"
#include "API/IServerEvent.h"
#include "AbstractModule.h"
#include <iostream>

class helloworld : public AbstractModule<IServerEvent>
{
    public:
        helloworld();
        ~helloworld();

        virtual ChainStatus     call(IModule::Event);
        ChainStatus             onServerStart();
        ChainStatus             onServerStop();

};

#endif // HELLOWORLD_H
