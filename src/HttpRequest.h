#ifndef __HTTPREQUEST_H__
# define __HTTPREQUEST_H__

#include <map>
#include <string>

class HttpRequest
{
    public:
        enum Command 
        {
            Get,
            Post,
            Header,
            Put,
            Delete,
            Options,
            Trace
        };

        enum Option
        {
            Host,
            From,
            UserAgent,
            ContentType,
            ContentLength,
            Date
        };

        HttpRequest();
        ~HttpRequest();
        void            setCommand(Command);
        void            setUri(std::string&);
        void            setProtocol(std::string&);
        void            appendOption(Option, std::string&);
        void            appendUriArgument(std::string&, std::string&);
        void            print();

    private:
        Command                             _command;
        std::map<Option, std::string>       _options;
        std::map<std::string, std::string>  _uriArguments;
        std::string                         _uri;
        std::string                         _protocol;
};

#endif /* !__HTTPREQUEST_H__ */
