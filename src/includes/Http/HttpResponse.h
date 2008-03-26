#ifndef __HTTPRESPONSE_H__
#define __HTTPRESPONSE_H__

#include <map>
#include <sstream>
#include <queue>

#include "File/IFile.h"
#include "HttpTransaction.h"
#include "Stream/IResponseStream.h"
#include "Stream/ErrorResponseStream.h"

class   HttpResponse : public HttpTransaction
{
    public:
        struct  KeyValue
        {
            int                         key;
            const char*                 value;
        };

        HttpResponse();
        virtual ~HttpResponse();
        
        void        setResponseStatus(int status);

        int                             getResponseStatus() const;
        const std::string               getResponseValue() const;
        size_t                          getContentLength() const;
        static const char*              getResponseStatusMessage(int key);

        void                            appendStream(IResponseStream*);
        std::queue<IResponseStream*>&   getStreams();
        void                            setError(ErrorResponseStream*);

    private:
        void                            clearStreams();

        static KeyValue                 ResponseStatus[];
        int                             _responseStatus;
        size_t                          _contentLength;
        std::string                     _mimeType;
        std::queue<IResponseStream*>    _streams;
};

#endif  /* !__HTTPRESPONSE_H__ */
