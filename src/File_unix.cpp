#include "File_unix.h"

File::File(std::string filename)
{
	stat(filename.c_str(), &_stat);
}

int		File::getSize() const
{
	return this->_stat.st_size;
}

time_t		File::getModifDate() const
{
	return this->_stat.st_mtime;
}

