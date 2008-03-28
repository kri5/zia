#include "DynLib.h"
#include <dlfcn.h>

DynLib::~DynLib()
{
	close();
}

void		DynLib::load(std::string filename)
{
	this->handle = dlopen(filename.c_str(), RTLD_LAZY);
}

void*		DynLib::sym(std::string symbol)
{
	return dlsym(this->handle, symbol.c_str());
}

void		DynLib::close()
{
	dlclose(this->handle);
}