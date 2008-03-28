#ifndef DYNLIB_H
#define DYNLIB_H

#include "zia.h"

#ifdef WIN32
 #include "DynLib_win32.h"
#else
 #include "DynLib_unix.h"
#endif

#endif // DYNLIB_H

