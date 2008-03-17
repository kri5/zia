#ifndef __HTTPREQUEST_H__
# define __HTTPREQUEST_H__

#include <map>
#include <string>

#include "Config.h"

class HttpRequest
{
    public:
        enum Command 
        {
            Get,
            Post,
            Head,
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
            Date,
            Accept,
            Connection
        };

        HttpRequest();
        ~HttpRequest();
        void                                        setCommand(Command);
        void                                        setUri(std::string&, bool=true);
        void                                        setProtocol(std::string&);
		void										setConfig(const Config* cfg);
        void                                        appendOption(Option, std::string&);
        void                                        appendUriArgument(std::string&, std::string&);
        void                                        appendBodyArgument(std::string&, std::string&);
        HttpRequest::Command                        getCommand() const;
        const std::string&                          getUri() const;
        const std::string&                          getProtocol() const;
        const std::map<Option, std::string>&        getOptions() const;
        std::string									getOption(Option) const;
		const Config*								getConfig() const;
        const std::map<std::string, std::string>&   getUriArguments() const; 
        bool                                        isUriRelative() const;
        void                                        clear();
        void                                        print();

    private:
        void                                        addDefaultHost();
        Command                                     _command;
        std::map<Option, std::string>               _options;
        std::map<std::string, std::string>          _uriArguments;
        std::map<std::string, std::string>          _bodyArguments;
        std::string                                 _uri;
        bool                                        _relativeUri;
        std::string                                 _protocol;
		Config*										_cfg;
};

#endif /* !__HTTPREQUEST_H__ */
