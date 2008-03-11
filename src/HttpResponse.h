#ifndef __HTTPRESPONSE_H__
#define __HTTPRESPONSE_H__

#include <istream>
#include <map>
#include <sstream>

#include "IFile.h"

class   HttpResponse
{
    public:
       // enum    Option
       // {
       //     Date,
       //     ContentType,
       //     ContentLength,
       //     MimeType
       // };

        struct  KeyValue
        {
            int                         key;
            const char*                 value;
        };

        HttpResponse();
        virtual ~HttpResponse();
        
        template <typename T>
        void        appendOption(std::string opt, T value)
        {
            std::ostringstream  s;

            s << value;
            this->_options[opt] = s.str();
        }
        void        appendOption(std::string, std::string&);
        void        setResponseStatus(int status);
        void        setProtocol(std::string&);
        const std::map<std::string, std::string>&     getOptions() const;

        const std::string&              getProtocol() const;
        int                             getResponseStatus() const;
        const std::string               getResponseValue() const;
        static const char*              getResponseStatusMessage(int key);

        // ios_base will be either an ostreamstring or a ifstream
        virtual std::iostream&          getContent() = 0;
        virtual bool                    completed() const = 0;

    private:
        static KeyValue                 ResponseStatus[];
        std::string                     _protocol;
        int                             _responseStatus;
        std::map<std::string, std::string>   _options;
};

#endif  /* !__HTTPRESPONSE_H__ */
