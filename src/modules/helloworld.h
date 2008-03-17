#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include "Modules/IModule.h"
#include <iostream>

class helloworld : public IModule
{
    public:
        helloworld();
        ~helloworld();
};

#endif // HELLOWORLD_H
