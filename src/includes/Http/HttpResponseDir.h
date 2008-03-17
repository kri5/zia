#ifndef HTTP_RESPONSE_DIR_H__
# define HTTP_RESPONSE_DIR_H__

#include <sstream>
#include <string>

#include "HttpResponse.h"
#include "DirectoryBrowser.h"

class HttpResponseDir : public HttpResponse
{
    public:
        HttpResponseDir(const HttpRequest*);
        ~HttpResponseDir();
        std::iostream&  getContent();
        bool            completed() const;
    private:
        std::stringstream*      _stream;
        DirectoryBrowser*       _browser;
};

#endif //HTTP_RESPONSE_DIR_H__

