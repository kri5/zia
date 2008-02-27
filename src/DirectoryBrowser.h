#ifndef __DIRECTORYBROWSER_H__
#define __DIRECTORYBROWSER_H__

#include <vector>
#include <sstream>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "FileSystem.h"

class DirectoryBrowser
{
    public:
        DirectoryBrowser(HttpRequest& request);
        ~DirectoryBrowser();

        HttpResponse&           getResponse();

    private:
        HttpRequest&            _request;
        FileSystem*             _fs;
        std::vector<IFile*>*    _fileList;
};

#endif // __DIRECTORYBROWSER_H__
