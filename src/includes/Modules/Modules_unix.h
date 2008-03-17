#ifndef MODULES_UNIX_H
#define MODULES_UNIX_H

#include <iostream>
#include <list>
#include <string>
#include "IModules.h"
#include "IModule.h"
#include "ModuleInfo.h"
#include "Logger.hpp"
#include "Singleton.hpp"

// API
#include "API/IServerStart.h"
#include "API/IPreReceive.h"
#include "API/IPostReceive.h"
#include "API/IPostBuild.h"
#include "API/IPreContent.h"
#include "API/IProcessContent.h"
#include "API/IServerQuit.h"

class   Modules : public IModules, public Singleton<Modules>
{
    public:
       virtual bool        load(std::string filename); 

    private:
        Modules();
        virtual ~Modules();
        std::list<ModuleInfo*>   _modules;

        friend class Singleton<Modules>;
};

#endif // MODULES_UNIX_H
