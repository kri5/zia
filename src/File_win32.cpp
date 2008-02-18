#include "File_win32.h"
#include "ZException.hpp"
#include "Time.h"

#include "MemoryManager.hpp"

File::File(std::string filename) : _filename(filename), _time(NULL)
{
	if (GetFileAttributesEx(filename.c_str(), GetFileExInfoStandard, &_attr) == 0)
		throw ZException<File>(INFO, File::Error::CantGetAttributes, "Probably because file does not exist");
}

File::~File()
{
	delete this->_time;
}

std::string		File::getFileName() const
{
	return this->_filename;
}

unsigned int				File::getSize() const
{
	if (this->_attr.nFileSizeHigh != 0)
		Logger::getInstance() << Logger::Warning << "Asked for file greater than 4G !" << Zia::Newline;
	return this->_attr.nFileSizeLow;
}

ITime*		File::getModifDate()
{
	if (this->_time == NULL)
		this->_time = new Time(this->_attr.ftLastWriteTime);
	return this->_time;
}