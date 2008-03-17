#ifndef MODULEINFO_H
#define MODULEINFO_H

#ifdef WIN32
    // Not available for now
    // #include "ModuleInfo_win32.h"
#else
    #include "ModuleInfo_unix.h"
#endif // WIN32

#endif // MODULEINFO_H
