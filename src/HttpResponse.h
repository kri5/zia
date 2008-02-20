#ifndef __HTTPRESPONSE_H__
#define __HTTPRESPONSE_H__

#include <string>
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

        void    setProtocol(std::string&);
        void    setResponseCode(std::string&);
        void    setResponseValue(std::string&);
        void    appendOption(HttpResponse::Option, std::string&);
    private:
        std::string                     _protocol;
        std::string                     _responseCode;
        std::string                     _responseValue;
        std::map<Option, std::string>   _options;
};

#endif  /* !__HTTPRESPONSE_H__ */
