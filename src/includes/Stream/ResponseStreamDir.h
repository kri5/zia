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
        std::iostream&  getContent();
        bool            completed() const;
        Type            getType() const;
        size_t          getSize() const;
    private:
        std::stringstream*      _stream;
        DirectoryBrowser*       _browser;
        Type                    _type;
        bool                    _good;
};

#endif //RESPONSE_STREAM_DIR_H__

