#ifndef IHTTPREQUEST_H__
# define IHTTPREQUEST_H__

#include "IHttpTransaction.h"
#include "IResponseStream.h"
#include "IConfig.h"
#include "IClientSocket.h"

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
            virtual ~IHttpRequest() {}

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
             *  Sets the uri's query string. A URI query is basically all there is 
             *  after the "?" of the URI
             *  @param query the uri query string
             */
            virtual void                                        setUriQuery(const std::string& query) = 0;

            /**
             *  Returns a string containing the query of the URI
             *  @return the uri arguments map
             */
            virtual const std::string&                          getUriQuery() const = 0;

            /**
             *  Will set the stream to the socket used by the client.
             *  This stream will be used by modules to read POST informations.
             *  @param stream The stream which is encapsulating the socket.
             */
            virtual void                                        setBodyStream(zAPI::IResponseStream* stream) = 0;

            /**
             *  Returns a stream to the socket where request is sent.
             *  You should use it to receive every form fields and files sent by POST.
             *  @return a IResponseStream* reading on the client socket.
             */
            virtual zAPI::IResponseStream*                      getBodyStream() const = 0;

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

            /**
             *  Return the current request ID
             *  Used for identifying requests within workflow, and eventually
             *  for the ModuleManager, in the case of a module list modification
             *  while a request is proceced. In this case, this id will
             *  allow the module manager to know which module list is associated with
             *  this request.
             *  @return the current request ID as an unsigned int.
             */
            virtual unsigned int                                getRequestId() const = 0;

            /**
             *  Set the current request's ID
             *  To set the request id (see getRequestId for more info on what is requestId)
             */
            virtual void                                        setRequestId(unsigned int id) = 0;

            /**
             *  Allow you to set the client which is sending request.
             *  @param clt the instance of IClientSocket corresponding to the client
             */
            virtual void                                        setClient(zAPI::IClientSocket* clt) = 0;

            /**
             *  Will return the client who has sent the request
             *  @return the IClientSocket instance.
             */
            virtual zAPI::IClientSocket*                        getClient() const = 0;
    };
}
#endif /* !IHTTPREQUEST_H__ */
