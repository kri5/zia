#ifndef FILE_UNIX_H__
# define FILE_UNIX_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

#include "Time/ITime.h"
#include "File/IFile.h"

class File : public IFile
{
	public:
		File(std::string, const char* path = NULL);
		~File();
        Error::Code     getError() const;
		std::string		getFileName() const;
        bool            isDirectory() const;
		unsigned int	getSize() const;
		ITime*			getModifDate();
        std::streamsize get(char* buff, size_t len);
		std::string		getExtension() const;
        bool            good() const;
        bool            eof() const;
        void            open();
        void            close();
        std::iostream*  getStream();
	private:
		std::string		_name;
        std::string     _filePath;
		struct stat		_stat;
		ITime*			_modifTime;
        std::fstream*   _stream;
        bool            _closed;
        Error::Code     _errorCode;
};

#endif //FILE_H__

