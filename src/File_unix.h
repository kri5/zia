#ifndef FILE_UNIX_H__
# define FILE_UNIX_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

#include "IFile.h"

class File : public IFile
{
	public:
		File(std::string);
		~File(){};
		int				getSize() const;
		time_t			getModifDate() const; 
	private:
		struct stat		_stat;
};

#endif //FILE_H__

