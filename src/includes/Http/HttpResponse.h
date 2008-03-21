#ifndef __HTTPRESPONSE_H__
#define __HTTPRESPONSE_H__

#include <istream>
#include <map>
#include <sstream>

#include "File/IFile.h"
#include "HttpTransaction.h"

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
        static const char*              getResponseStatusMessage(int key);

        // ios_base will be either an ostreamstring or a ifstream
        virtual std::iostream&          getContent() = 0;
        virtual bool                    completed() const = 0;

    private:
        static KeyValue                 ResponseStatus[];
        int                             _responseStatus;
};

#endif  /* !__HTTPRESPONSE_H__ */
