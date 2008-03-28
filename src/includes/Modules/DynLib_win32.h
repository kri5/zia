#ifndef DYNLIB_WIN32_H
#define DYNLIB_WIN32_H

#include <string>
#include "IDynLib.h"

class DynLib : public IDynLib
{
public:
	~DynLib();
	void		load(std::string filename);
	void*		sym(std::string symbol);
	void		close();
	
private:
	HMODULE		handle;
};

#endif // DYNLIB_WIN32_H