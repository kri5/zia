#include "IClientSocket.h"
#include "IMainSocket.h"

const char* IClientSocket::Error::Msg[] = {
    "Unknow error",
    "Send error",
    "Receive error",
    "Disconnect error"
};

const char* IMainSocket::Error::Msg[] = {
	"Unknow error",
	"Can't create socket",
	"Can't bind socket",
	"Listen error",
	"Accept error"
};
