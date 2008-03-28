#ifndef IDYNLIB_H
#define IDYNLIB_H

#include <string>

class IDynLib
{
public:
	virtual ~IDynLib(){};
	void		load(std::string filename);
	void		sym(std::string symbol);
	void		close();
};

#endif // IDYNLIB_H

