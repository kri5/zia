#ifndef __HTTPERROR_H__
#define __HTTPERROR_H__

#include <sstream>
#include <string>
#include <istream>

#include "HttpRequest.h"
#include "HttpResponse.h"

class   HttpError : public HttpResponse
{
    public:
        HttpError(int status, HttpRequest& request);
        HttpError(int status, HttpRequest* request);
        virtual ~HttpError() throw();
        std::iostream&              getContent();
        bool                        completed() const;
    private:
        HttpRequest&                _request;
        int                         _status;
        std::string                 _message;
        std::stringstream*          _content;
};


#endif // __HTTPERROR_H__
