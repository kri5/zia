#include "zia.h"
#include "FileSystem_unix.h"
#include "ZException.hpp"
#include "Logger.hpp"
#include "File.h"

#include "MemoryManager.hpp"

FileSystem::FileSystem(std::string path) : _path(path), _files(NULL)
{
}

FileSystem::~FileSystem()
{
	if (this->_files != NULL)
	{
		std::list<IFile*>::iterator		it = this->_files->begin();
		std::list<IFile*>::iterator		end = this->_files->end();

		while (it != end)
		{
			delete *it;
			++it;
		}
		this->_files->clear();
		delete this->_files;
	}
}

std::list<IFile*>*		FileSystem::getFileList()
{
	if (this->_files == NULL)
	{
		DIR*			dir = opendir(this->_path.c_str());
		struct dirent*	file;

		if (!dir)
		{
			closedir(dir);
			throw ZException<FileSystem>(INFO, FileSystem::Error::OpenDir, strerror(errno));
		}
		this->_files = new std::list<IFile*>;
		while ((file = readdir(dir)))
		{
			if (file->d_name[0] == '.' &&
					(file->d_name[1] == 0 || (file->d_name[1] == '.' && file->d_name[2] == 0)))
				continue ;
			this->_files->push_back(new File(file->d_name, this->_path.c_str()));
		}
		closedir(dir);
	}
	return this->_files;
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


