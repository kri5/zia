#include "Modules/DynLib.h"

DynLib::~DynLib()
{
	close();
}

bool		DynLib::load(const std::string& filename)
{
	if ((this->handle = LoadLibrary(filename.c_str())) == 0)
        return false;
    return true;
}

void*		DynLib::sym(const std::string& symbol)
{
	return GetProcAddress(this->handle, symbol.c_str());
}

void		DynLib::close()
{
	FreeLibrary(this->handle);
}

const char*       DynLib::lastError()
{
    return strerror(GetLastError());
}
