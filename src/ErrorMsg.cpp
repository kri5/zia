#include "IClientSocket.h"
#include "MainSocket.h"
#include "NetworkID.h"
#include "Vhost.h"
#include "Port.h"
#include "FileSystem.h"
#include "File.h"
#include "Server.h"

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
	"Invalid config",
	"Port can't be wildcarded"
};

const char* Vhost::Error::Msg[] = {
	"Unknown error",
	"Asked for an invalid paramter from a Virtual Host"
};

const char*	Port::Error::Msg[] = {
	"Unknown error",
	"Given port is invalid"
};

const char* Server::Error::Msg[] = {
	"Unknown error",
	"Select error"
};

// Archi dependent part.
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
#ifdef WIN32

#include "Socket_win32.h"
const char*	Socket::Error::Msg[] = {
	"Unknown error",
	"Can't initialize socket librairie (WSAstartup)"
};

#include "File_win32.h"

const char* File::Error::Msg[] = {
	"Unknown error",
	"Can't get file's attributes"
};

#include "FileSystem_win32.h"

const char* FileSystem::Error::Msg[] = {
	"Unknown error",
	"Can't have valid file handle"
};

#include "Time_win32.h"

const char* Time::Error::Msg[] = {
	"Unknown error",
	"Can't convert from FILETIME to SYSTEMTIME"
};

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

#else

const char* File::Error::Msg[] = {
	"Unknown error",
	"No such file or directory"
};

const char* FileSystem::Error::Msg[] = {
	"Unknown error",
	"Can't opendir for reading",
	"Can't read directory with readdir",
	"Can't stat file"
};
#endif

