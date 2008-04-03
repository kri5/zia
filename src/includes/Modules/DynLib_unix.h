#ifndef DYNLIB_UNIX_H
#define DYNLIB_UNIX_H

#include <string>
#include "API/IDynLib.h"

class DynLib : public zAPI::IDynLib
{
public:
    DynLib();
	~DynLib();
	bool		    load(const std::string& filename);
	void*		    sym(const std::string& symbol);
	void		    close();
    const char*     lastError();

private:
	void*		    handle;
    std::string*    _err;
};

#endif // DYNLIB_UNIX_H

