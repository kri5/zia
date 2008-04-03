#ifndef ERRORRESPONSESTREAM_H__
#define ERRORRESPONSESTREAM_H__

#include <sstream>
#include <string>
#include <istream>

#include "Http/HttpRequest.h"
#include "API/IResponseStream.h"

class   ErrorResponseStream : public zAPI::IResponseStream
{
    public:
        ErrorResponseStream(int status, HttpRequest& request);
        ErrorResponseStream(int status, HttpRequest* request);
        virtual ~ErrorResponseStream() throw();
        std::iostream&              getContent();
        int                         getStatus() const;
        bool                        completed() const;
        Type                        getType() const;
        size_t                      getSize() const;
        bool                        good() const;
    private:
        void                        setContent();

        HttpRequest&                _request;
        int                         _status;
        std::string                 _message;
        std::stringstream*          _content;
        Type                        _type;
};


#endif // ERRORRESPONSESTREAM_H__
