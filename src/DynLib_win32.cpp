#include "Modules/DynLib.h"

DynLib::~DynLib()
{
	close();
}

void		DynLib::load(std::string filename)
{
	this->handle = LoadLibrary(filename.c_str());
}

void*		DynLib::sym(std::string symbol)
{
	return GetProcAddress(this->handle, symbol.c_str());
}

void		DynLib::close()
{
	FreeLibrary(this->handle);
}