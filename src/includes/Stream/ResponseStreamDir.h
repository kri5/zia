#ifndef RESPONSE_STREAM_DIR_H__
# define RESPONSE_STREAM_DIR_H__

#include <sstream>
#include <string>

#include "Http/HttpResponse.h"
#include "DirectoryBrowser.h"

class ResponseStreamDir : public zAPI::IResponseStream
{
    public:
        ResponseStreamDir(const HttpRequest*);
        ~ResponseStreamDir();
        bool            good() const;
        size_t          read(char*, size_t);
        bool            completed() const;
        size_t          getSize() const;
    private:
        std::stringstream*      _stream;
        DirectoryBrowser*       _browser;
        bool                    _good;
};

#endif //RESPONSE_STREAM_DIR_H__

