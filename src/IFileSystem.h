#ifndef IFILESYSTEM_H__
# define IFILESYSTEM_H__

#include <list>

class	IFileSystem
{
	public:
		enum	Rights
		{
			Read = 1,
			Exec = 2,
			Write = 4
		};
		struct	FileInfo
		{
			std::string		filename;
			int				size;
			int				modifDate;
		};
		virtual ~IFileSystem();
		/// Will return files as a std::list.
		virtual std::list<FileInfo>*		getFileList() const = 0;
		/// Will return true if the given file exists.
		virtual bool						checkFileExistence() const = 0;
		/// Will return true if given file has read rights.
		virtual bool						checkReadRights() const = 0;
		/// Will return true if given file has exec rights.
		virtual bool						checkExecRights() const = 0;
		/** 
		 * Will check if given file as the given rights.
		 * Rights will be given as binary mask from IFileSystem::Rights (ie : Rights::Read || Rights::Write)
		 **/
		virtual bool						checkRights(int) const = 0;
};

#endif //IFILESYSTEM_H__

