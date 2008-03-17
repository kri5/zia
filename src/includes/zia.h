#ifndef ZIA_H__
#define ZIA_H__

#ifdef WIN32
 #include <WinSock2.h> // Must be defined in this order, WinSock2 _before_ windows.h
 #include <Windows.h>
 #ifdef FD_SETSIZE
  #undef FD_SETSIZE
 #endif
 #define FD_SETSIZE 512 // By default on windows FD are limited to 64.
 #define poll WSAPoll
 #define sleep(x) Sleep((x)*1000)
	/// Newline for Win32
	namespace Zia
	{
		static const char*	Newline = "\r\n";
	}
#else //For unix
	/// Newline for unix
	namespace Zia
	{
		//This might be unused in some files, but if so we don't care. It's used elsewhere
		__attribute__((unused)) static const char*	Newline = "\n";
	}
#endif //WIN32

#endif // __ZIA_H__

