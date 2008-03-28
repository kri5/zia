#ifndef DYNLIB_WIN32_H
#define DYNLIB_WIN32_H

#include <string>
#include "IDynLib.h"

class DynLib : public IDynLib
{
public:
	~DynLib();
	bool		load(const std::string& filename);
	void*		sym(const std::string& symbol);
	void		close();
    char*       lastError();
	
private:
	HMODULE		handle;
};

#endif // DYNLIB_WIN32_H
