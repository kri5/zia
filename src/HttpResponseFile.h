#ifndef HTTP_RESPONSE_FILE_H__
# define HTTP_RESPONSE_FILE_H__

#include <istream>

#include "HttpResponse.h"
#include "IFile.h"

class HttpResponseFile : public HttpResponse
{
    public:
        HttpResponseFile(IFile*);
        ~HttpResponseFile();
        std::iostream&          getContent();
        bool                    completed() const;
    private:
        IFile*                 _file;
};

#endif //HTTP_RESPONSE_FILE_H__

