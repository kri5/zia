#ifndef SOCKET_WIN32_H__
#define SOCKET_WIN32_H__

#include "zia.h"
#include "ISocket.h"

/// Generic Win32 socket class.
class Socket : public ISocket
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
	void 			close(bool shutdown);
	const ISocket&	operator>>(struct pollfd& pdfs) const;
	bool            isSet(const struct pollfd& pdfs) const;
	void            setPollFlag(int);
	bool			isClosed() const;
protected:
	WSADATA wsaData;
	SOCKET listenSocket;
	int				_pollFlag;
	bool			_closed;
};

#endif // __SOCKET_WIN32_H__

