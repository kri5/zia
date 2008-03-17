#ifndef FILESYSTEM_H__
# define FILESYSTEM_H__

#ifdef WIN32
 #include "FileSystem_win32.h"
#else
 #include "FileSystem_unix.h"
#endif //WIN32

#endif //FILESYSTEM_H__

