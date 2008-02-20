#ifndef __HTTPERROR_H__
#define __HTTPERROR_H__

#include <exception>
#include <sstream>
#include <string>
#include "HttpRequest.h"
#include "HttpResponse.h"

class   HttpError : public std::exception
{
    public:
        HttpError(int status, HttpRequest& request);
        virtual ~HttpError() throw();
        virtual const char*         what() const throw();
        HttpResponse&               getResponse() const;

    private:
        HttpRequest&                _request;
        int                         _status;
        std::string                 _message;
};


#endif // __HTTPERROR_H__
