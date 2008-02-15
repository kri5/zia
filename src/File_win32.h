#ifndef FILE_WIN32_H__
# define FILE_WIN32_H__

#include <unistd.h>
#include <string>

#include "IFile.h"

class File : public IFile
{
	public:
		/// Error definitions for File class.
		struct	Error
		{
			enum	Code
			{
				Unknown,
				NoSuchFile
			};
			static const char*	Msg[];
		};
		File(std::string);
		~File(){};
		int				getSize() const;
		time_t			getModifDate() const; 
	private:

};

#endif //FILE_WIN32_H__

