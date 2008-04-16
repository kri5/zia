#include "SSL.h"
#include "SSLClientSocket.h"

extern "C" zAPI::IModule* create()
{
    return new ModSSL;
}

extern "C" void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "ModSSL (Secure Socket Layer)";
}

extern "C" int  version()
{
    return 1;
}

ModSSL::ModSSL()
{
    std::cout << "ModSSL is initializing..." << std::endl;
    SSL_library_init(); // load encryption & hash algorithms for SSL
    SSL_load_error_strings(); // load the error strings for good error reporting
}

ModSSL::~ModSSL()
{

}

zAPI::IClientSocket*        ModSSL::call(zAPI::IModule::Event event, SOCKET socket)
{
    if (event == zAPI::IModule::onAcceptEvent)
        return onAccept(socket);
}

zAPI::IClientSocket*        ModSSL::onAccept(SOCKET socket)
{
    return new SSLClientSocket(socket); 
}

zAPI::IModule::ChainStatus  ModSSL::onReceive(char* buf, size_t length)
{
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus  ModSSL::onSend(char* buf, size_t length)
{
    return zAPI::IModule::CONTINUE;
}

int                         ModSSL::getPriority(zAPI::IModule::Event event) const
{
    return 1;
}

