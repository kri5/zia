#include <sstream>
#include <io.h>

#include "FileSystem_win32.h"
#include "File.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

FileSystem::FileSystem(std::string path) : _path(path), _files(NULL)
{
}

FileSystem::~FileSystem()
{
	if (this->_files)
	{
		std::vector<IFile*>::iterator		it = this->_files->begin();
		std::vector<IFile*>::iterator		end = this->_files->end();
		while (it != end)
		{
			delete *it;
			++it;
		}
		this->_files->clear();
		delete this->_files;
	}
}

std::vector<IFile*>*			FileSystem::getFileList()
{
	if (this->_files == NULL)
	{
		WIN32_FIND_DATA			files;
		HANDLE					search;
		BOOL					res;
		std::string				fileName = this->_path + "/*";
		
		this->_files = new std::vector<IFile*>;
		if ((search = FindFirstFile("*", &files)) == INVALID_HANDLE_VALUE)
			throw ZException<FileSystem>(INFO, FileSystem::Error::InvalidHandle, "Probably because file does not exist.");
		res = TRUE;
		while (res)
		{
			this->_files->push_back(new File(files.cFileName));
			res = FindNextFile(search, &files);
		}
		FindClose(search);
	}
	return this->_files;
}

bool		FileSystem::checkRights(std::string name, int mode) const
{
	std::ostringstream	str;
	str << this->_path << "/" << name;
	return (_access_s(str.str().c_str(), mode) == 0);
}

bool		FileSystem::checkFileExistence(std::string name) const
{
	return this->checkRights(name, FileSystem::Existence);
}

bool		FileSystem::checkReadRights(std::string name) const
{
	return this->checkRights(name, FileSystem::Existence);
}

bool		FileSystem::checkExecRights(std::string name) const
{
	return this->checkRights(name, FileSystem::Exec);
}