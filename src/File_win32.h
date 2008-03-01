#ifndef FILE_WIN32_H__
# define FILE_WIN32_H__

#include <string>

#include "zia.h"
#include "IFile.h"
#include "WinBase.h"
#include "ITime.h"

class File : public IFile
{
	public:
		/// Error definitions for File class.
		struct	Error
		{
			enum	Code
			{
				Unknown,
				CantGetAttributes
			};
			static const char*	Msg[];
		};
		File(const std::string&, const char* path = NULL);
		~File();
		std::string		getFileName() const;
		unsigned int	getSize() const;
		ITime*			getModifDate();
		bool            isDirectory() const;
		std::string		getExtension() const;
		void			open();
		void			close();
		std::streamsize	get(char* buff, size_t len);
		bool			good() const;
		bool			eof() const;
	private:
		std::string					_filename;
		std::string					_filePath;
		WIN32_FILE_ATTRIBUTE_DATA	_attr;
		ITime*						_time;
		std::ifstream*				_stream;
		bool						_closed;
};

#endif //FILE_WIN32_H__

