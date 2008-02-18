#ifndef IFILE_H__
# define IFILE_H__

#include <string>

#include "ITime.h"

/// File interface, for having file informations
class	IFile
{
	public:
		virtual ~IFile(){};
		virtual std::string				getFileName() const = 0;
		virtual unsigned int			getSize() const = 0;
		virtual ITime*					getModifDate() = 0;
		//virtual std::string		getNextChunk() const = 0;
};

#endif //IFILE_H__

