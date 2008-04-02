#ifndef __IHTTPRESPONSE_H__
# define __IHTTPRESPONSE_H__

#include <queue>

#include "API/IHttpTransaction.h"
#include "API/IResponseStream.h"

class IHttpResponse : public virtual IHttpTransaction
{
    public:

        /**
         *  The Interface virtual destructor
         */
        virtual ~IHttpResponse() {};

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
         *  Returns a description of the HttpResponse status code
         *  @return status code's description
         */
        virtual std::string                     getResponseValue() const = 0;

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
         *  Sets the current stream providing the response content
         *  @param stream the stream to set current
         */
        virtual void                            setCurrentStream(std::iostream* stream) = 0;

        /**
         *  Returns the current stream providing response content data
         *
         *  @return  the current stream
         */
        virtual std::iostream&                  getCurrentStream() = 0;

	      /**
         *  Stream is the potentially new stream to add. If NULL, nothing will be added
         *  if flushStream is true, every other streams will be deleted. If headerIncluded is true, the double
         *  carriage return must be in stream. Nothing will be added automatically.
         *
         *  @param stream the response content stream
         *  @param flushStream whether the reponse should contains set streams or not
         *  @params headerInStream whether the header should be contained in the stream or not 
         *                          (if not, double \\r\\n will automatically be added)
         */
        virtual void                            skipToSend(IResponseStream* stream, bool flushStream, bool headerInStream) = 0;
};

#endif /* !__IHTTPRESPONSE_H__ */
