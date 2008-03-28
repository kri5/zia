#ifndef DYNLIB_UNIX_H
#define DYNLIB_UNIX_H

#include <string>
#include "IDynLib.h"

class DynLib : public IDynLib
{
public:
	~DynLib();
	void		load(std::string filename);
	void		sym(std::string symbol);
	void		close();

private:
	void*		handle;
};

#endif // DYNLIB_UNIX_H