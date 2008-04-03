#ifndef __IHTTPREQUEST_H__
# define __IHTTPREQUEST_H__

#include "API/IHttpTransaction.h"
#include "API/IConfig.h"

#include <string>
#include <map>

namespace zAPI
{
    /// Describes requirements for a HTTP response.
    class IHttpRequest : public virtual IHttpTransaction
    {
        public:

            /**
             *  The interface virtual destructor
             */
            virtual ~IHttpRequest() {};

            /**
             *  Sets the request URI and whether the uri is relative or not.
             *  The relative flag is set to tru by default
             *  @param uri the request uri
             *  @relative whether the uri is relative or not (default true)
             */
            virtual void                                        setUri(const std::string& uri, bool relative= true) = 0;

            /**
             *  Returns the URI
             *  @return the uri
             */
            virtual const std::string&                          getUri() const = 0;

            /**
             *  Sets a uri argument with a key / value pair passed in parameter
             *  @param key the uri argument key
             *  @param value the uri argument value
             */
            virtual void                                        setUriArgument(const std::string& key, const std::string& value) = 0;

            /**
             *  Returns a map containing all the uri arguments
             *  @return the uri arguments map
             */
            virtual const std::map<std::string, std::string>&   getUriArguments() const = 0;

            /**
             *  Sets a body argument with a key / value pair passed in paramater
             *  @param key a body argument key
             *  @param value a body argument value
             */
            virtual void                                        setBodyArgument(const std::string& key, const std::string& value) = 0;

            /**
             *  Returns a map containing all the body arguments
             *  @return the body arguments map
             */
            virtual const std::map<std::string, std::string>&   getBodyArguments() const = 0;

            /**
             *  Returns the body argument value corresponding to the key passed in parameter.
             *  If the key is not found, NULL will be returned instead
             *  @param key a boby argument key
             *  @return a body argument value
             */
            virtual const std::string&                          getBodyArgument(const std::string& key) const = 0;

            /**
             *  Sets a custom parameter that will not be used in the server.
             *  Those parameters must only be used for modules purposes
             *  @param key a parameter key
             *  @param value a parameter value (casted as void*)
             */
            virtual bool                                        setParam(const std::string& key, void* value) = 0;

            /**
             *  Returns the param value matching the key passed in parameter.
             *  If the key is not found, NULL will be returned instead
             *  @param key a param key
             *  @return a param value (casted as void*)
             */
            virtual void*                                       getParam(const std::string&) const = 0;

            /**
             *  Sets the request command (GET, POST, PUT ...)
             *  @param command a request command
             */
            virtual void                                        setCommand(const std::string& command) = 0;

            /**
             *  Returns the request command
             *  @return a request command
             */
            virtual const std::string&                          getCommand() const = 0;

            /**
             *  Sets the current configuration (can be request specific)
             *  @param config a configuration object
             */
            virtual void                                        setConfig(IConfig* config) = 0;

            /**
             *  Returns the current configuration (can be request specific)
             *  @return a configuration object
             */
            virtual IConfig*                                    getConfig() const = 0;

            virtual unsigned int                                getRequestId() const = 0;
    };
}
#endif /* !__IHTTPREQUEST_H__ */
