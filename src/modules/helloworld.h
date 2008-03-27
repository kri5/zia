#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include "API/IModule.h"
#include "AbstractModule.h"
#include <iostream>

class helloworld : public AbstractModule
{
    public:
        helloworld();
        ~helloworld();
};

#endif // HELLOWORLD_H
