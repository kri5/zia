#ifndef FILESYSTEM_WIN32_H__
# define FILESYSTEM_WIN32_H__

#include <windows.h>
#include <list>

#include "zia.h"
#include "IFile.h"
#include "IFileSystem.h"

class	FileSystem : public IFileSystem
{
	public:
		FileSystem(std::string);
		virtual 					~FileSystem();
		std::vector<IFile>*			getFileList() const;
		bool						checkFileExistence() const;
		bool						checkReadRights() const;
		bool						checkExecRights() const;
		bool						checkRights(int) const;
	private:
		std::string					_filename;
};

#endif //FILESYSTEM_WIN32_H__

