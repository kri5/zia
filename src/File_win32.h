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
	private:
		std::string					_filename;
		WIN32_FILE_ATTRIBUTE_DATA	_attr;
		ITime*						_time;
};

#endif //FILE_WIN32_H__

