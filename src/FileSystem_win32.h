#ifndef FILESYSTEM_WIN32_H__
# define FILESYSTEM_WIN32_H__

class	FileSystem : public IFileSystem
{
	public:
		FileSystem(std::string);
		virtual 					~FileSystem();
		std::list<FileInfo>*		getFileList() cnst;
		bool						checkFileExistence() const;
		bool						checkReadRights() const;
		bool						checkExecRights() const;
		bool						checkRights(int) const;
	private:
		std::string					_filename;
};

#endif //FILESYSTEM_WIN32_H__

