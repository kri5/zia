#ifndef __HTTPREQUEST_H__
# define __HTTPREQUEST_H__

#include <map>
#include <string>

#include "Http/HttpTransaction.h"
#include "Config.h"
#include "API/IHttpRequest.h"

class HttpRequest : public IHttpRequest, public HttpTransaction
{
    public:
        HttpRequest();
        ~HttpRequest();
        void                                        setCommand(const std::string&);
        void                                        setUri(const std::string&, bool = true);
		void										setConfig(const Config* cfg);
        void                                        setUriArgument(const std::string&, const std::string&);
        void                                        setBodyArgument(const std::string&, const std::string&);
        std::string                                 getCommand() const;
        const std::string&                          getUri() const;
		const Config*								getConfig() const;
        const std::map<std::string, std::string>&   getUriArguments() const; 
        bool                                        isUriRelative() const;
        void                                        clear();
        void                                        print();
        bool                                        setParam(const std::string&, void*);
        void*                                       getParam(const std::string&) const;

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
