#ifndef DYNLIB_UNIX_H
#define DYNLIB_UNIX_H

#include <string>
#include "API/IDynLib.h"

class DynLib : public IDynLib
{
public:
	~DynLib();
	bool		load(const std::string& filename);
	void*		sym(const std::string& symbol);
	void		close();
    char*       lastError();

private:
	void*		handle;
};

#endif // DYNLIB_UNIX_H

