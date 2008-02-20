#ifndef __HTTPRESPONSE_H__
#define __HTTPRESPONSE_H__

#include <iostream>
#include <map>

class   HttpResponse
{
    public:
        HttpResponse();
        ~HttpResponse();
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


        void                            setProtocol(std::string&);
        void                            setResponseStatus(int status);
        void                            setResponseValue(std::string&);
        void                            appendOption(HttpResponse::Option, std::string&);
        void                            setContent(std::iostream content);
        static keyvalue                 ResponseStatus[];
        static const char*              getResponseStatusMessage(int key);

    private:
        std::string                     _protocol;
        int                             _responseStatus;
        std::string                     _responseValue;
        std::map<Option, std::string>   _options;
};

#endif  /* !__HTTPRESPONSE_H__ */
