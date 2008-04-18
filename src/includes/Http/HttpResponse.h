#ifndef __HTTPRESPONSE_H__
#define __HTTPRESPONSE_H__

#ifdef WIN32
#pragma warning(disable: 4250)
#endif

#include <map>
#include <queue>

#include "API/IResponseStream.h"
#include "File/IFile.h"
#include "HttpTransaction.h"
#include "API/IResponseStream.h"
#include "Stream/ErrorResponseStream.h"
#include "API/IHttpResponse.h"

class   HttpResponse : public zAPI::IHttpResponse, public HttpTransaction
{
    public:
        struct  KeyValue
        {
            int                         key;
            const char*                 value;
        };

        HttpResponse();
        virtual ~HttpResponse();
        
        void                                    setResponseStatus(int status);
        int                                     getResponseStatus() const;

        const std::string&                      getResponseValue() const;
        void                                    setResponseValue(const std::string&);
        static const char*                      getResponseStatusMessage(int key);

        void                                    appendStream(zAPI::IResponseStream*);
        std::queue<zAPI::IResponseStream*>&     getStreams();
        void                                    setError(ErrorResponseStream*);
        size_t                                  getContentLength() const;

        zAPI::IResponseStream*                  getCurrentStream();
		void							        skipToSend(zAPI::IResponseStream* stream, bool flushStream, bool headerInStream);
        void                                    setHeaderInStream(bool);
		bool							        isInSendMode() const;
		bool							        headerInStream() const;

    private:
        void                                    clearStreams();

        static KeyValue                         ResponseStatus[];
        int                                     _responseStatus;
        std::string                             _responseValue;
        size_t                                  _contentLength;
        std::string                             _mimeType;
        std::queue<zAPI::IResponseStream*>      _streams;
        std::iostream*                          _currentStream;
		bool							        _sendMode;
		bool							        _headerInStream;
};

#endif  /* !__HTTPRESPONSE_H__ */
