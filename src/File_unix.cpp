#include "File_unix.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

File::File(std::string filename)
{
	if (stat(filename.c_str(), &_stat) < 0)
	{
		throw ZException<File>(INFO, File::Error::NoSuchFile, filename.c_str());
	}
}

int		File::getSize() const
{
	return this->_stat.st_size;
}

time_t		File::getModifDate() const
{
	return this->_stat.st_mtime;
}

