#ifndef FILE_WIN32_H__
# define FILE_WIN32_H__

#include <string>

#include "zia.h"
#include "IFile.h"
#include "WinBase.h"

class File : public IFile
{
	public:
		/// Error definitions for File class.
		struct	Error
		{
			enum	Code
			{
				Unknown,
				CantGetAttributes,
				
			};
			static const char*	Msg[];
		};
		File(std::string);
		~File(){};
		std::string		getFileName() const;
		unsigned int	getSize() const;
		time_t			getModifDate() const; 
	private:
		std::string					_filename;
		WIN32_FILE_ATTRIBUTE_DATA	_attr;
};

#endif //FILE_WIN32_H__

