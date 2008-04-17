#ifndef IFILE_H__
# define IFILE_H__

#include <string>

#include "Time/ITime.h"

/// File interface, for having file informations
class	IFile
{
	public:
		/// Error definitions for File class.
		struct	Error
		{
			enum	Code
			{
                None = -1,
				Unknown,
				NoSuchFile,
                PermissionDenied
			};
			static const char*	Msg[];
		};
		virtual ~IFile(){};
		virtual Error::Code             getError() const = 0;
        virtual const std::string&		getFileName() const = 0;
        virtual const std::string&		getFullFileName() const = 0;
        virtual bool                    isDirectory() const = 0;
		virtual unsigned int			getSize() const = 0;
		virtual ITime*					getModifDate() = 0;
		virtual std::string				getExtension() const = 0;
        virtual void                    open() = 0;
        virtual void                    close() = 0;
        virtual std::streamsize         get(char* buff, size_t len) = 0;
        virtual size_t                  read(char* buff, size_t len) = 0;
        virtual bool                    good() const = 0;
        virtual bool                    eof() const = 0;
        virtual std::iostream*          getStream() = 0;
		//virtual std::string		getNextChunk() const = 0;
};

#endif //IFILE_H__

