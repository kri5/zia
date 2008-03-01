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
        virtual bool                    isDirectory() const = 0;
		virtual unsigned int			getSize() const = 0;
		virtual ITime*					getModifDate() = 0;
		virtual std::string				getExtension() const = 0;
        virtual void                    open() = 0;
        virtual void                    close() = 0;
        virtual std::streamsize         get(char* buff, size_t len) = 0;
        virtual bool                    good() const = 0;
        virtual bool                    eof() const = 0;
		//virtual std::string		getNextChunk() const = 0;
};

#endif //IFILE_H__

