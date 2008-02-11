#include "IClientSocket.h"
#include "IMainSocket.h"
#include "NetworkID.h"
#include "Vhost.h"

const char* IClientSocket::Error::Msg[] = {
    "Unknown error",
    "Send error",
    "Receive error",
    "Disconnect error"
};

const char* IMainSocket::Error::Msg[] = {
	"Unknown error",
	"Can't create socket",
	"Can't bind socket",
	"Listen error",
	"Accept error"
};

const char*	NetworkID::Error::Msg[] = {
	"Unknown error",
	"Invalid config"
};

const char* Vhost::Error::Msg[] = {
	"Unknown error",
	"Asked for an invalid paramter from a Virtual Host"
};

#ifdef WIN32
#include "Socket_win32.h"
const char*	Socket::Error::Msg[] = {
	"Unknown config",
	"Can't initialize socket librairie (WSAstartup)"
};
#endif
