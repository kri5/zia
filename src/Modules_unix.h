#ifndef MODULES_UNIX_H
#define MODULES_UNIX_H

#include <iostream>
#include <list>
#include "IModules.h"
#include "IModule.h"
#include "ModuleInfo.h"
#include "Logger.hpp"

class   Modules : public IModules
{
    public:
        Modules();
        virtual ~Modules();
        virtual bool        load(std::string filename); 

    private:
        std::list<ModuleInfo*>   _modules;

};

#endif // MODULES_UNIX_H
