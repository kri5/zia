#ifndef __DIRECTORYBROWSER_H__
#define __DIRECTORYBROWSER_H__

#include <list>
#include <sstream>
#include <string>

#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "FileSystem/FileSystem.h"

class DirectoryBrowser
{
    public:
        DirectoryBrowser(const HttpRequest& request, std::stringstream* stream);
        ~DirectoryBrowser();

        bool                            get();
    private:
        const HttpRequest&              _request;
        FileSystem*                     _fs;
        std::list<IFile*>*              _fileList;
        bool                            _first;
        std::stringstream*              _stream;
};

#endif // __DIRECTORYBROWSER_H__
