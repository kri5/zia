#ifndef __ZIA_H__
#define __ZIA_H__

namespace Zia
{
#ifdef WIN32
 #include <WinSock2.h> // Must be defined in this order, WinSock2 _before_ windows.h
 #include <Windows.h>
 #ifdef FD_SETSIZE
  #undef FD_SETSIZE
 #endif
 #define FD_SETSIZE 512 // By default on windows FD are limited to 64.
 #define sleep(x) Sleep((x)*1000)
	/// Newline for Win32
	static const char*	Newline = "\r\n";
#else //For unix
	/// Newline for unix
	static const char*	Newline = "\n";
#endif //WIN32
}
#endif // __ZIA_H__

