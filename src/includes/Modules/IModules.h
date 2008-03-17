#ifndef IMODULES_H
#define IMODULES_H

#include <iostream>

class   IModules
{
    public:
        virtual ~IModules() {};
        virtual bool    load(std::string filename) = 0;
};

#endif // IMODULES_H

