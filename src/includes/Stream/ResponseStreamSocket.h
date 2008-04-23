#ifndef RESPONSESTREAM_SOCKET_H__
# define RESPONSESTREAM_SOCKET_H__

#include "API/IClientSocket.h"
#include "API/IResponseStream.h"

class   ResponseStreamSocket : public zAPI::IResponseStream
{
    public:
        struct  Error
        {
            enum    Code
            {
                Unknown,
                Size
            };
            static const char*  Msg[];
        };
        ResponseStreamSocket(zAPI::IClientSocket*, char*, size_t);
        virtual ~ResponseStreamSocket();
        virtual size_t          read(char*, size_t);
        bool                    completed() const;
        bool                    good() const;
        size_t                  getSize() const;
    private:
        int                     sockPoll() const;
        zAPI::IClientSocket*    _socket;
        char*                   _previouslyReadDatas;
        char*                   _previouslyReadDatasPtr;
        size_t                  _prevBufferSize;
};

#endif //RESPONSESTREAM_SOCKET_H__

