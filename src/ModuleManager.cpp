#include "Modules/ModuleManager.h"
#include <dlfcn.h>

ModuleManager::ModuleManager()
{

}

ModuleManager::~ModuleManager()
{
    for (std::list<IModule*>::iterator it = _modules.begin(); it != _modules.end(); ++it)
        delete *it;
}

bool            ModuleManager::load(std::string filename)
{
    // Loading the module
    void* handle = dlopen(filename.c_str(), RTLD_LAZY);
    if (!handle)
    {
        Logger::getInstance() << Logger::Error << dlerror() << '\n' << Logger::Flush;
        return false;
    }

    // First, we extract the needed symbols.
    name_t* module_name = (name_t*) dlsym(handle, "name");
    destroy_t* module_destroy = (destroy_t*) dlsym(handle, "destroy");
    create_t* module_create = (create_t*) dlsym(handle, "create");

    // If any of these is null, a symbol is missing.
    if (!module_name || !module_create || !module_destroy)
    {
        Logger::getInstance() << Logger::Error << "Can't load symbol: "
            << dlerror() << Logger::Flush;
        return false;
    }

    // Creating a ModuleInfo object.
    //ModuleInfo* mi = new ModuleInfo(handle);
    
    // Identifying which "event" class the module implement.
    IModule* ptr = module_create();
    //if (dynamic_cast<IServerStart*>(ptr))
    //    mi->support.push_back(ServerStart); 
    //if (dynamic_cast<IPreReceive*>(ptr))
    //    mi->support.push_back(PreReceive);
    //if (dynamic_cast<IPostReceive*>(ptr))
    //    mi->support.push_back(PostReceive);
    //if (dynamic_cast<IPostBuild*>(ptr))
    //    mi->support.push_back(PostBuild);
    //if (dynamic_cast<IPreContent*>(ptr))
    //    mi->support.push_back(PreContent);
    //if (dynamic_cast<IProcessContent*>(ptr))
    //    mi->support.push_back(ProcessContent);
    //if (dynamic_cast<IServerQuit*>(ptr))
    //    mi->support.push_back(ServerQuit);
    module_destroy(ptr);

    // Now we add it to the list of loaded modules.
    //_modules.push_back(mi);

    Logger::getInstance() << Logger::Info << "Module " << module_name() << " loaded." << Logger::Flush;
    return true;
}

