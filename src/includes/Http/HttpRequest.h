#ifndef __HTTPREQUEST_H__
# define __HTTPREQUEST_H__

#ifdef WIN32
#pragma warning(disable: 4250)
#endif

#include <map>
#include <string>

#include "Http/HttpTransaction.h"
#include "API/IConfig.h"
#include "API/IHttpRequest.h"

class HttpRequest : public IHttpRequest, public HttpTransaction
{
    public:
        HttpRequest();
        ~HttpRequest();
        void                                        setUri(const std::string&, bool = true);
        const std::string&                          getUri() const;
        void                                        setUriArgument(const std::string&, const std::string&);
        const std::map<std::string, std::string>&   getUriArguments() const; 

        void                                        setBodyArgument(const std::string&, const std::string&);
        const std::map<std::string, std::string>&   getBodyArguments() const;
        const std::string&                          getBodyArgument(const std::string&) const;

        bool                                        setParam(const std::string&, void*);
        void*                                       getParam(const std::string&) const;

        void                                        setCommand(const std::string&);
        const std::string&                          getCommand() const;

		void										setConfig(IConfig* cfg);
		IConfig*								    getConfig() const;
        bool                                        isUriRelative() const;
        void                                        clear();
        void                                        print();

    private:
        void                                        addDefaultHost();
        std::string                                 _command;
        std::map<std::string, std::string>          _uriArguments;
        std::map<std::string, std::string>          _bodyArguments;
        std::map<std::string, void*>                _params;
        std::string                                 _uri;
        bool                                        _relativeUri;
		IConfig*									_cfg;
};

#endif /* !__HTTPREQUEST_H__ */
