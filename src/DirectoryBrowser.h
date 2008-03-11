#ifndef __DIRECTORYBROWSER_H__
#define __DIRECTORYBROWSER_H__

#include <list>
#include <sstream>

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "FileSystem.h"

class DirectoryBrowser
{
    public:
        DirectoryBrowser(const HttpRequest& request, std::stringstream* stream);
        ~DirectoryBrowser();

        void                            get();
    private:
        const HttpRequest&              _request;
        FileSystem*                     _fs;
        std::list<IFile*>*              _fileList;
        bool                            _first;
        std::stringstream*              _stream;
};

#endif // __DIRECTORYBROWSER_H__
