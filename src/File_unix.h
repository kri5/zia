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
		std::string		getExtension() const;
	private:
		std::string		_name;
		struct stat		_stat;
		ITime*			_modifTime;
};

#endif //FILE_H__

