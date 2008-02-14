#include "zia.h"
#include "FileSystem_unix.h"
#include "ZException.hpp"
#include "Logger.hpp"

#include "MemoryManager.hpp"

FileSystem::FileSystem(std::string path) : _path(path)
{
}

FileSystem::~FileSystem()
{
}

std::vector<FileSystem::FileInfo>*		FileSystem::getFileList() const
{
	DIR*			dir = opendir(this->_path.c_str());
	struct dirent*	file;
	struct stat		s;

	if (!dir)
	{
		closedir(dir);
		throw ZException<FileSystem>(INFO, FileSystem::Error::OpenDir, strerror(errno));
	}
	std::vector<FileSystem::FileInfo>*	vect = new std::vector<FileSystem::FileInfo>;
	FileSystem::FileInfo		info;
	while ((file = readdir(dir)))
	{
		if (file->d_name[0] == '.' &&
				(file->d_name[1] == 0 || (file->d_name[1] == '.' && file->d_name[2] == 0)))
			continue ;
		std::string filePath = this->_path + '/' + file->d_name;
		if (stat(filePath.c_str(), &s) == -1)
		{
			//be sure to close directory ressource.
			throw ZException<FileSystem>(INFO, FileSystem::Error::Stat, filePath.c_str());
		}
		info.filename = file->d_name;
		info.size = s.st_size;
		info.modifDate = s.st_mtime;
		vect->push_back(info);
	}
	closedir(dir);
	return vect;
}


bool						FileSystem::checkFileExistence(std::string fileName) const
{
	struct stat				s;
	const std::string&			filePath = this->_path + "/" + fileName;

	if (stat(filePath.c_str(), &s) == -1)
	{
		return false;
	}
	return true;
}

bool						FileSystem::checkReadRights(std::string fileName) const
{
	const std::string&			filePath = this->_path + "/" + fileName;

	if (access(filePath.c_str(), FileSystem::Read) < 0)
	{
		if (errno == EACCES)
			return false;
	}
	return true;
}

bool						FileSystem::checkExecRights(std::string fileName) const
{
	const std::string&			filePath = this->_path + "/" + fileName;

	if (access(filePath.c_str(), FileSystem::Exec) < 0)
	{
		if (errno == EACCES)
			return false;
	}
	return true;
}

bool						FileSystem::checkRights(std::string fileName, int rights) const
{
	const std::string&			filePath = this->_path + "/" + fileName;

	if (access(filePath.c_str(), rights) < 0)
	{
		if (errno == EACCES)
			return false;
	}
	return true;
}


