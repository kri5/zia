#include "SSL.h"
#include "SSLClientSocket.h"

extern "C" DLLEXP zAPI::IModule* create()
{
    return new ModSSL;
}

extern "C" DLLEXP void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" DLLEXP std::string name()
{
    return "ModSSL (Secure Socket Layer)";
}

extern "C" DLLEXP float  version()
{
    return 1;
}

ModSSL::ModSSL()
{
    std::cout << "ModSSL is initializing..." << std::endl;
    SSL_library_init(); // load encryption & hash algorithms for SSL
    SSL_load_error_strings(); // load the error strings for good error reporting
    std::cout << "Initialized." << std::endl;
}

ModSSL::~ModSSL()
{

}

zAPI::IClientSocket*        ModSSL::onAccept(SOCKET socket, const std::string& address, int port, zAPI::IConfig* config)
{
	if (!config->getParam("SSLCertificateFile") || !config->getParam("SSLCertificateKeyFile"))
		return NULL;
    if (port == 8443)
    {
        std::cout << "SSL enabled" << std::endl;
        return new SSLClientSocket(socket, address, port, config);
    }
    return NULL;
}

zAPI::IModule::ChainStatus  ModSSL::onReceive(const char* buf, size_t length)
{
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus  ModSSL::onSend(const char* buf, size_t length)
{
    return zAPI::IModule::CONTINUE;
}

int                         ModSSL::getPriority(zAPI::IModule::Event event) const
{
    return 1;
}

