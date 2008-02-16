#include "File_win32.h"
#include "ZException.hpp"

File::File(std::string filename) : _filename(filename)
{
	if (GetFileAttributesEx(filename.c_str(), GetFileExInfoStandard, &_attr) == 0)
		throw ZException<File>(INFO, File::Error::CantGetAttributes, (const char*)GetLastError());
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

//FIXME
time_t File::getModifDate() const{return 0;}