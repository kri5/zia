#include "File_unix.h"
#include "ZException.hpp"
#include "Time.h"

#include "MemoryManager.hpp"

File::File(std::string filename, const char* path) : _name(filename), _modifTime(NULL)
{
	std::string			filePath;
	if (path)
	{
		filePath = path;
		filePath += "/";
		filePath += filename;
	}
	else
		filePath = filename;

	if (stat(filePath.c_str(), &_stat) < 0)
	{
		throw ZException<File>(INFO, File::Error::NoSuchFile, filename.c_str());
	}
}

File::~File()
{
	if (this->_modifTime)
		delete this->_modifTime;
}

std::string		File::getFileName() const
{
	return this->_name;
}

unsigned int		File::getSize() const
{
	return this->_stat.st_size;
}

ITime*		File::getModifDate()
{
	if (this->_modifTime == NULL)
		this->_modifTime = new Time(this->_stat.st_mtime);
	return this->_modifTime;
}

bool        File::isDirectory() const
{
    return S_ISDIR(this->_stat.st_mode);
}

std::string		File::getExtension() const
{
	return this->_filename.substr(this->_filename.rfind("."));
}
