#include "DirectoryBrowser.h"
#include "Utils/UrlString.h"

DirectoryBrowser::DirectoryBrowser(const HttpRequest& request, 
        std::stringstream* stream) :
    _request(request), _first(true), _stream(stream)
{
    // Read directory path from request
    // If it isn't a directory, throw 500
    _fs = new FileSystem(request.getConfig()->getParam("DocumentRoot") + request.getUri());
    _fileList = _fs->getFileList();
}

DirectoryBrowser::~DirectoryBrowser()
{
    delete this->_fs;
}

void           DirectoryBrowser::get()
{

    std::string parent = _request.getUri();

    // Remove trailing slash, if any.
    if (parent.size() > 1 && parent[parent.size() - 1] == '/')
        parent.erase(parent.size() - 1);

    // Find the parentdir path
    size_t found = parent.rfind("/");
    if (found > 0)
        parent.erase(found);
    else
        parent.erase(1);

    *(this->_stream) << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n";
    *(this->_stream) << "<html>\n<head>\n<title>Index of " << _request.getUri() << "</title></head>\n";
    *(this->_stream) << "<body><h1>Index of " << _request.getUri() << "</h1>\n";
    *(this->_stream) << "<pre>Name                    Last modified      Size  Description<hr>\n";
    *(this->_stream) << "<a href=\"" << parent << "\">Parent Directory</a>                            -\n";

    std::list<IFile*>::iterator      it = this->_fileList->begin();
    std::list<IFile*>::iterator      ite = this->_fileList->end();

    while (it != ite)
    {
        std::string path;
        if (_request.getUri() != "/")
            path = _request.getUri() + "/";

        IFile* f = (*it);

        // Url encoding.
        std::string fullpath = path + f->getFileName();
        fullpath = UrlString::urlEncode(fullpath);

        *(this->_stream) << "[   ]<a href=\"" << fullpath << "\">" << f->getFileName() << "</a>     ";
        *(this->_stream) << f->getModifDate()->getStr() << "   ";
        *(this->_stream) << f->getSize() << "  \n";
        ++it;
 
    }
    *(this->_stream) << "<hr></pre><address>ZiaHttpd Server at http://" << 
        this->_request.getHeaderOption("Host").substr(0, this->_request.getHeaderOption("Host").find(":")) << 
        this->_request.getUri() << " Port ";
    if (this->_request.getHeaderOption("Host").find(":") != std::string::npos)
        *(this->_stream) << this->_request.getHeaderOption("Host").substr(this->_request.getHeaderOption("Host").find(":") + 1);
    else
        *(this->_stream) << this->_request.getConfig()->getDefaultPort();
    *(this->_stream) << "</address>\n</body></html>";
}

