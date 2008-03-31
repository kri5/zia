#ifndef IDYNLIB_H
#define IDYNLIB_H

#include <string>

class IDynLib
{
public:
    virtual ~IDynLib(){};
	virtual bool		load(const std::string& filename) = 0;
	virtual void*		sym(const std::string& symbol) = 0;
	virtual void		close() = 0;
    virtual const char* lastError() = 0;
};

#endif // IDYNLIB_H

