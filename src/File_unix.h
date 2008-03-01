#ifndef FILE_UNIX_H__
# define FILE_UNIX_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

#include "ITime.h"
#include "IFile.h"

class File : public IFile
{
	public:
		/// Error definitions for File class.
		struct	Error
		{
			enum	Code
			{
				Unknown,
				NoSuchFile
			};
			static const char*	Msg[];
		};
		File(std::string, const char* path = NULL);
		~File();
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
	private:
		std::string		_name;
        std::string     _filePath;
		struct stat		_stat;
		ITime*			_modifTime;
        std::ifstream*  _stream;
        bool            _closed;
};

#endif //FILE_H__

