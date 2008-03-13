#include "Modules.h"
#include <dlfcn.h>

Modules::Modules()
{

}

Modules::~Modules()
{
    for (std::list<ModuleInfo*>::iterator it = _modules.begin(); it != _modules.end(); it++)
        delete *it;
}

bool            Modules::load(std::string filename)
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
    ModuleInfo* mi = new ModuleInfo(handle);
    // Now we add it to the list of loaded modules.
    _modules.push_back(mi);

    Logger::getInstance() << Logger::Info << "Module " << module_name() << " loaded." << Logger::Flush;
    return true;
}

