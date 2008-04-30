#ifndef MODSSL_H
#define MODSSL_H

#include "API/IModule.h"
#include "API/INetwork.h"
#include "API/IConfig.h"

#ifdef _WIN32
 #define DLLEXP __declspec(dllexport)
#else
 #define DDLEXP
#endif // !_WIN32

class ModSSL : public zAPI::IModule, public zAPI::INetwork
{
    public:
        ModSSL();
        virtual ~ModSSL();

        zAPI::IClientSocket*        onAccept(SOCKET socket, const std::string& address, int port, zAPI::IConfig* config);
        zAPI::IModule::ChainStatus  onReceive(const char* buf, size_t length);
        zAPI::IModule::ChainStatus  onSend(const char* buf, size_t length);
        int                         getPriority(zAPI::IModule::Event event) const;
};

#endif // MODSSL_H
