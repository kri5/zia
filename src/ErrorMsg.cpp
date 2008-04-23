#include "API/IClientSocket.h"
#include "Sockets/MainSocket.h"
#include "Sockets/ClientSocket.h"
#include "Network/NetworkID.h"
#include "Network/Vhost.h"
#include "Network/Port.h"
#include "FileSystem/FileSystem.h"
#include "File/File.h"
#include "Server.h"
#include "Config.h"
#include "Workflow/Pool.h"
#include "Stream/ResponseStreamSocket.h"

const char* ClientSocket::Error::Msg[] = {
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
    "Can't bind socket",
	"Select error"
};

const char*	Config::Error::Msg[] = {
	"Unknown error",
	"Can't access this out of root config"
};

const char* Pool::Manager::Error::Msg[] = {
    "Unknown error",
    "Poll() error"
};

const char* ResponseStreamSocket::Error::Msg[] = {
    "Unknown error",
    "Can't ask for a socket stream size"
};

// Archi dependent part.
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
#ifdef WIN32

#include "File/File_win32.h"

const char* File::Error::Msg[] = {
	"Unknown error",
	"Can't get file's attributes"
};

#include "FileSystem/FileSystem_win32.h"

const char* FileSystem::Error::Msg[] = {
	"Unknown error",
	"Can't have valid file handle"
};

#include "Time/Time_win32.h"

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

