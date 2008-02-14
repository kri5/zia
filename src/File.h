#ifndef FILE_H__
# define FILE_H__

#ifdef WIN32
 #include "File_win32.h"
#else
 #include "File_unix.h"
#endif

#endif //FILE_H__

