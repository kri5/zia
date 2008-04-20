#ifndef __INETWORK_H__
# define __INETWORK_H__

#include "IModule.h"
#include "IClientSocket.h"
#include "IConfig.h"

namespace zAPI
{
    /**
     *  This hook interface forces you to implement tree event methods.
     *  The first one (onAccept) allows you to change the socket used for the transaction.
     *  The second one (onReceive) allows you to access the received data.
     *  The third one (onSend) allows you to access the data to be sent.
     */
    /// Interface for module who wants to hook on Network events
    class INetwork
    {
        public:
            /**
             *  The interface virtual destructor
             */
            virtual ~INetwork() {}

            /**
             *  The accept event.
             *  This method will be called just after a accept has been performed.
             *  It lets you change the socket used for the transaction with your own 
             *  IClientSocket implementation. Typically, you can use this event to replace
             *  the classic IClientSocket by it's SLL implementation.
             *  @param socket the socket built from the accept call
             *  @param address the client's IP
             *  @param port the port used by the client. (To know if it's a SSL port or not)
             *  @param config the configuration associated with the connection
             *  @return a IClientSocket that will be used until the end of the transaction
             */
            virtual IClientSocket*  onAccept(SOCKET socket, const std::string& address, int port, zAPI::IConfig* config) = 0;

            /**
             *  The receive event.
             *  This method will be called after each reveive performed on the IClientSocket.
             *  You'll got the received datas on a buffer and it's length as parameters.
             *  That way, you'll be allowed to modify all incomming datas.
             *  @param buf the buffer containg the receivded datas
             *  @param length the datas length
             *  @return a ChainStatus
             */
            virtual IModule::ChainStatus    onReceive(const char* buf, size_t length) = 0;

            /**
             *  The send event.
             *  This method will be called before each send performed on the IClientSocket.
             *  You'll got the datas to be sent along with it's length as parameters.
             *  That way, you'll be allowed to modify all outgoing datas.
             *  @param buf the buffer containing the datas to be sent
             *  @param length the datas length
             *  @return a ChainStatus
             */
            virtual IModule::ChainStatus    onSend(const char* buf, size_t length) = 0;
    };
}

#endif /* !__INETWORK_H__ */
