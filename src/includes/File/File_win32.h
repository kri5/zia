#ifndef FILE_WIN32_H__
# define FILE_WIN32_H__

#include <string>

#include "zia.h"
#include "File/IFile.h"
#include "WinBase.h"
#include "Time/ITime.h"

class File : public IFile
{
	public:
		/// Error definitions for File class.
		File(const std::string&, const char* path = NULL);
		~File();
		std::string		getFileName() const;
		Error::Code     getError() const;
		unsigned int	getSize() const;
		ITime*			getModifDate();
		bool            isDirectory() const;
		std::string		getExtension() const;
		void			open();
		void			close();
		std::streamsize	get(char* buff, size_t len);
		bool			good() const;
		bool			eof() const;
		std::iostream*	getStream();
	private:
		std::string					_filename;
		std::string					_filePath;
		WIN32_FILE_ATTRIBUTE_DATA	_attr;
		ITime*						_time;
		std::fstream*				_stream;
		bool						_closed;
		Error::Code					_errorCode;
};

#endif //FILE_WIN32_H__

