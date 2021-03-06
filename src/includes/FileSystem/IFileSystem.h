#ifndef IFILESYSTEM_H__
# define IFILESYSTEM_H__

#include <list>
#include <string>

#include "File/IFile.h"

class	IFileSystem
{
	public:
		virtual ~IFileSystem(){};
		/// Will return files as a std::list.
		virtual std::list<IFile*>*		    getFileList(const char* = NULL) = 0;
		/// Will return true if the given file exists.
		virtual bool						checkFileExistence(std::string) const = 0;
		/// Will return true if given file has read rights.
		virtual bool						checkReadRights(std::string) const = 0;
		/// Will return true if given file has exec rights.
		virtual bool						checkExecRights(std::string) const = 0;
		/** 
		 * Will check if given file as the given rights.
		 * Rights will be given as binary mask from IFileSystem::Rights (ie : Rights::Read || Rights::Write)
		 **/
		virtual bool						checkRights(std::string, int) const = 0;
};

#endif //IFILESYSTEM_H__

