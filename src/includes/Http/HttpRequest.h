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

class HttpRequest : public virtual zAPI::IHttpRequest, public HttpTransaction
{
    public:
        HttpRequest();
        virtual ~HttpRequest();
        void                                        setUri(const std::string&, bool = true);
        const std::string&                          getUri() const;
        //void                                        setUriArgument(const std::string&, const std::string&);
        //const std::map<std::string, std::string>&   getUriArguments() const; 
        void                                        setUriQuery(const std::string&);
        const std::string&                          getUriQuery() const;

        bool                                        setParam(const std::string&, void*);
        void*                                       getParam(const std::string&) const;

        zAPI::IResponseStream*                      getBodyStream() const;
        void                                        setBodyStream(zAPI::IResponseStream* stream);

        void                                        setCommand(const std::string&);
        const std::string&                          getCommand() const;

		void										setConfig(zAPI::IConfig* cfg);
        zAPI::IConfig*								getConfig() const;
        bool                                        isUriRelative() const;
        void                                        clear();
        void                                        print();

        unsigned int                                getRequestId() const;
        void                                        setRequestId(unsigned int);

    private:
        void                                        addDefaultHost();
        std::string                                 _command;
        std::string                                 _uriArguments;
        std::map<std::string, void*>                _params;
        std::string                                 _uri;
        bool                                        _relativeUri;
        zAPI::IConfig*								_cfg;
        unsigned int                                _reqId;
        zAPI::IResponseStream*                      _bodyStream;
};

#endif /* !__HTTPREQUEST_H__ */
