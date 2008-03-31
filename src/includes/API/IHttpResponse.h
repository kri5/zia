#ifndef __IHTTPRESPONSE_H__
# define __IHTTPRESPONSE_H__

#include "API/IHttpTransaction.h"
#include "API/IResponseStream.h"

class IHttpResponse : public virtual IHttpTransaction
{
    public:
        virtual ~IHttpResponse() {};

        virtual void                setResponseStatus(int) = 0;
        virtual int                 getResponseStatus() const = 0;

        virtual const std::string   getResponseValue() const = 0;
        static const char*          getResponseStatusMessage(int);

        virtual std::iostream&      getContent() = 0;
		/// stream is the potentially new stream to add. If NULL, nothing will be added
		/// if flushStream is true, every other streams will be deleted. If headerIncluded is true, the double
		/// carriage return must be in stream. Nothing will be added automatically.
		virtual void                skipToSend(IResponseStream* stream, bool flushStream, bool headerInStream) = 0;
};

#endif /* !__IHTTPRESPONSE_H__ */
