#ifndef __IRECEIVEREQUEST_H__
# define __IRECEIVEREQUEST_H__

#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

/**
 *  This hook interface forces you to implements two event methods.
 *  The first one (onPreReceive) will be called before receiving any data on the IClientSocket.
 *  The second one (onPostReceive) will be called after receiving the datas.
 */
class IReceiveRequest : public IModule
{
    public:
        /**
         *  The interface virtual destructor
         */
        virtual ~IReceiveRequest() {};

        /**
         *  the preReceive event.
         *  This method will be called just before receiving datas from the IClientSocket.
         *  @param request the IHttpRequest of the transaction
         *  @param response the IHttpResponse of the transaction
         *  @return a ChainStatus
         */
        virtual ChainStatus onPreReceive(IHttpRequest&, IHttpResponse&) = 0;

        /**
         *  the postReceive event.
         *  This method will be called just after receiving some datas from the IClientSocket.
         *  @param request the IHttpRequest of the transaction
         *  @param response the IHttpResponse of the transaction
         *  @return a ChainStatus
         */
        virtual ChainStatus onPostReceive(IHttpRequest&, IHttpResponse&) = 0;
};

#endif /* !__IRECEIVEREQUEST_H__ */
