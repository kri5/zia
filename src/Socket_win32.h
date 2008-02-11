#ifndef SOCKET_WIN32_H__
#define SOCKET_WIN32_H__

#include "zia.h"

/// Generic Win32 socket class.
class Socket
{
public:
	struct	Error
	{
		enum	Code
		{
			Unknown,
			Initialize
		};
		static const char*	Msg[];
	};
	/// Will initialize WSA
	Socket();
	/// Clean WSA
	~Socket();
	
	/// Close the connection with auto-shutdown support.
	void close(bool shutdown) const;
protected:
	WSADATA wsaData;
	SOCKET listenSocket;
};

#endif // __SOCKET_WIN32_H__

