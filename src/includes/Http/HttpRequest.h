#ifndef __HTTPREQUEST_H__
# define __HTTPREQUEST_H__

#include <map>
#include <string>

#include "Config.h"

class HttpRequest
{
    public:
        HttpRequest();
        ~HttpRequest();
        void                                        setCommand(std::string);
        void                                        setUri(std::string&, bool = true);
        void                                        setProtocol(std::string&);
		void										setConfig(const Config* cfg);
        void                                        appendOption(std::string, std::string&);
        void                                        appendUriArgument(std::string&, std::string&);
        void                                        appendBodyArgument(std::string&, std::string&);
        std::string                                 getCommand() const;
        const std::string&                          getUri() const;
        const std::string&                          getProtocol() const;
        const std::map<std::string, std::string>&   getOptions() const;
        std::string									getOption(std::string) const;
		const Config*								getConfig() const;
        const std::map<std::string, std::string>&   getUriArguments() const; 
        bool                                        isUriRelative() const;
        void                                        clear();
        void                                        print();

    private:
        void                                        addDefaultHost();
        std::string                                 _command;
        std::map<std::string, std::string>          _options;
        std::map<std::string, std::string>          _uriArguments;
        std::map<std::string, std::string>          _bodyArguments;
        std::string                                 _uri;
        bool                                        _relativeUri;
        std::string                                 _protocol;
		Config*										_cfg;
};

#endif /* !__HTTPREQUEST_H__ */
