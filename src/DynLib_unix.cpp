#include "Modules/DynLib.h"
#include <dlfcn.h>

DynLib::~DynLib()
{
	close();
}

bool		DynLib::load(const std::string& filename)
{
	if ((this->handle = dlopen(filename.c_str(), RTLD_LAZY)) == 0)
        return false;
    return true;
}

void*		DynLib::sym(const std::string& symbol)
{
	return dlsym(this->handle, symbol.c_str());
}

void		DynLib::close()
{
	dlclose(this->handle);
}

char*       DynLib::lastError()
{
    return dlerror();
}
