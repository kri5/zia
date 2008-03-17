#ifndef FILESYSTEM_WIN32_H__
# define FILESYSTEM_WIN32_H__

#include <list>
#include <string>

#include "zia.h"
#include "IFile.h"
#include "IFileSystem.h"

class	FileSystem : public IFileSystem
{
	public:
		struct	Error
		{
			enum	Code
			{
				Unknown,
				InvalidHandle
			};
			static const char*	Msg[];
		};
		enum	Rights
		{
			Existence = 0,
			Read = 2,
			Exec = 4,
			Write = 6
		};
		FileSystem(std::string);
		virtual 					~FileSystem();
		/// Don't bother with getFileList() memory allocation, FileSystem::~FileSystem() will do it automaticaly.
		std::list<IFile*>*			getFileList();
		bool						checkFileExistence(std::string) const;
		bool						checkReadRights(std::string) const;
		bool						checkExecRights(std::string) const;
		bool						checkRights(std::string, int) const;
	private:
		std::string					_path;
		std::list<IFile*>*			_files;
};

#endif //FILESYSTEM_WIN32_H__

