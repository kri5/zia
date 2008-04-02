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
         *  @param int StatusCode
         */
        virtual void                            setResponseStatus(int) = 0;

        /**
         *  Returns the HttpResponse status code
         *  @return status code
         */
        virtual int                             getResponseStatus() const = 0;

        /**
         *  Returns a description of the HttpResponse status code
         *  @return std:string status code description
         */
        virtual std::string                     getResponseValue() const = 0;

        /**
         *  Appends a IResponseStream that will be used 
         *  while sending the response content.
         *  @param IResponseStream the stream to append
         */
        virtual void                            appendStream(IResponseStream*) = 0;

        /**
         *  Returns all the stream composing the reponse content
         *  @return std::queue<IResponseStream*>& the queue containing all the response streams
         */
        virtual std::queue<IResponseStream*>&   getStreams() = 0;

        /**
         *  Sets the current stream providing the response content
         *  @params std::iostream* the stream to set current
         */
        virtual void                            setCurrentStream(std::iostream*) = 0;

        /**
         *  Returns the current stream providing response content data
         *  @return std::iostream& the current stream
         */
        virtual std::iostream&                  getCurrentStream() = 0;

	      /**
         *  Stream is the potentially new stream to add. If NULL, nothing will be added
         *  if flushStream is true, every other streams will be deleted. If headerIncluded is true, the double
         *  carriage return must be in stream. Nothing will be added automatically.
         */
    		virtual void                            skipToSend(IResponseStream* stream, bool flushStream, bool headerInStream) = 0;
};

#endif /* !__IHTTPRESPONSE_H__ */
