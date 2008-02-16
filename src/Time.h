#ifndef TIME_H__
#define TIME_H__

#ifdef WIN32
#include "Time_win32.h"
#else
#include "Time_unix.h"
#endif //WIN32

#endif //TIME_H__