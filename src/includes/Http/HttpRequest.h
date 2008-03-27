#ifndef __HTTPREQUEST_H__
# define __HTTPREQUEST_H__

#include <map>
#include <string>

#include "HttpTransaction.h"
#include "Config.h"

class HttpRequest : public HttpTransaction
{
    public:
        HttpRequest();
        ~HttpRequest();
        void                                        setCommand(std::string);
        void                                        setUri(std::string&, bool = true);
		void										setConfig(const Config* cfg);
        void                                        appendUriArgument(std::string&, std::string&);
        void                                        appendBodyArgument(std::string&, std::string&);
        std::string                                 getCommand() const;
        const std::string&                          getUri() const;
        const std::string&                          getProtocol() const; //<<
		const Config*								getConfig() const;
        const std::map<std::string, std::string>&   getUriArguments() const; 
        bool                                        isUriRelative() const;
        void                                        clear();
        void                                        print();
        bool                                        setParam(std::string, void*);
        void*                                       getParam(std::string) const;

    private:
        void                                        addDefaultHost();
        std::string                                 _command;
        std::map<std::string, std::string>          _uriArguments;
        std::map<std::string, std::string>          _bodyArguments;
        std::map<std::string, void*>                _params;
        std::string                                 _uri;
        bool                                        _relativeUri;
		Config*										_cfg;
};

#endif /* !__HTTPREQUEST_H__ */
