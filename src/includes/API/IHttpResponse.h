#ifndef IHTTPRESPONSE_H__
# define IHTTPRESPONSE_H__

#include <queue>

#include "IHttpTransaction.h"
#include "IResponseStream.h"

namespace zAPI
{
    /// Describes requirements for a Http Response.
    class IHttpResponse : public virtual IHttpTransaction
    {
        public:

            /**
             *  The Interface virtual destructor
             */
            virtual ~IHttpResponse() {}

            /**
             *  Sets the HttpResponse status code
             *  @param statusCode the reponse's status code
             */
            virtual void                            setResponseStatus(int statusCode) = 0;

            /**
             *  Returns the HttpResponse status code
             *  @return the response's status code
             */
            virtual int                             getResponseStatus() const = 0;

            /**
             *  Sets the request's response value
             *  @param value The reponse value (a text describing how the request has been handled)
             */
            virtual void                            setResponseValue(const std::string& value) = 0;
            /**
             *  Returns a description of the HttpResponse status code
             *  @return status code's description
             */
            virtual const std::string&              getResponseValue() const = 0;

            /**
             *  Appends a IResponseStream that will be used 
             *  while sending the response content.
             *  @param stream the stream to append
             */
            virtual void                            appendStream(IResponseStream* stream) = 0;

            /**
             *  Returns all the streams composing the reponse content
             *  @return a queue containing all the response streams
             */
            virtual std::queue<IResponseStream*>&   getStreams() = 0;

            /**
             *  Returns the current stream providing response content data
             *
             *  @return  the current stream
             */
            virtual IResponseStream*                getCurrentStream() = 0;

            /**
             *  This method will allow you to skip to response sending(skip every step(s) between this event and
             *  the response sending). You can just add an ending stream, or 
             *  flush every streams already set, and set yours as the only stream to send.
             *  If your stream contains header, you can change header sending mode by passing true as third parameter.
             *
             *  @param stream the response content stream
             *  @param flushStream whether the reponse should contains set streams or not
             *  @params headerInStream whether the header should be contained in the stream or not 
             *                          (if not, double \\r\\n will automatically be added)
             */
            virtual void                            skipToSend(IResponseStream* stream, bool flushStream, bool headerInStream) = 0;

            /**
             *  Will set headerInStream status.
             *  headerInStream is a state flag which let the server know if it should automatically add a
             *  \r\n to notify the end of the HTTP header, or if it will be contained in the response
             *  body itself (as an exemple : PHP will add some header fields and a \r\n, so headerInStream
             *  will have to be set to true.
             *  Default value is false
             *  @param headerInStream the headerInStream flag new value.
             */
            virtual void                            setHeaderInStream(bool headerInStream) = 0;
            /**
             *  Will allow you to know if headerInStream is true or false
             *  @return headerInStream value
             */
            virtual bool							getHeaderInStream() const= 0;
    };
}

#endif /* !IHTTPRESPONSE_H__ */
