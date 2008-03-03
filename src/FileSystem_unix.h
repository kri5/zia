#ifndef FILESYSTEM_UNIX_H__
# define FILESYSTEM_UNIX_H__

#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "IFile.h"
#include "IFileSystem.h"

class	FileSystem : public IFileSystem
{
	public:
		enum	Rights
		{
			Read = R_OK,
			Exec = X_OK,
			Write = W_OK
		};
		struct	Error
		{
			enum	Code
			{
				Unknown,
				OpenDir,
				ReadDir,
				Stat
			};
			static const char*	Msg[];
		};
		FileSystem(std::string);
		virtual 					~FileSystem();
		std::list<IFile*>*		    getFileList();
		bool						checkFileExistence(std::string) const;
		bool						checkReadRights(std::string) const;
		bool						checkExecRights(std::string) const;
		bool						checkRights(std::string, int) const;
	private:
		std::string					_path;
		std::list<IFile*>*		    _files;
};

#endif //FILESYSTEM_UNIX_H__

