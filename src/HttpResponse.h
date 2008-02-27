#ifndef __HTTPRESPONSE_H__
#define __HTTPRESPONSE_H__

#include <iostream>
#include <map>

class   HttpResponse
{
    public:
        enum    Option
        {
            Date,
            ContentType,
            ContentLength
        };

        typedef struct
        {
            int                         key;
            const char*                 value;
        } keyvalue;

        HttpResponse();
        ~HttpResponse();
        
        void                            setProtocol(std::string&);
        void                            setResponseStatus(int status);
        void                            appendOption(HttpResponse::Option, std::string&);
        void                            setContent(std::istream* content);
        void                            setContentLength(unsigned int size);
        void                            setMimetype(std::string mimetype);
        
        const std::string&              getProtocol();
        int                             getResponseStatus();
        const std::string               getResponseValue();
        std::istream&                   getContent();
        unsigned int                    getContentLength();
        std::string                     getMimetype();

        static keyvalue                 ResponseStatus[];
        static const char*              getResponseStatusMessage(int key);

    private:
        std::string                     _protocol;
        int                             _responseStatus;
        std::map<Option, std::string>   _options;
        std::istream*                   _content;
        unsigned int                    _contentLength;
        std::string                     _mimetype;
};

#endif  /* !__HTTPRESPONSE_H__ */
