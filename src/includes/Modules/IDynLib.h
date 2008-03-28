#ifndef IDYNLIB_H
#define IDYNLIB_H

#include <string>

class IDynLib
{
public:
	virtual ~IDynLib(){};
	bool		load(const std::string& filename);
	void		sym(const std::string& symbol);
	void		close();
    char*       lastError();
};

#endif // IDYNLIB_H

