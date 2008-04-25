#include "DirectoryBrowser.h"
#include "Utils/UrlString.h"
#include <iostream>
#include <iomanip>

DirectoryBrowser::DirectoryBrowser(const HttpRequest& request, 
        std::iostream* stream) :
    _request(request), _first(true), _stream(stream)
{
    // Read directory path from request
    // DocumentRoot has to be defined. This is normally checked in Init
    _fs = new FileSystem(*(request.getConfig()->getParam("DocumentRoot")) + request.getUri());
    _fileList = _fs->getFileList();
}

DirectoryBrowser::~DirectoryBrowser()
{
    delete this->_fs;
}

bool           DirectoryBrowser::get()
{
    if (this->_fileList == NULL)
        return false;
    std::string parent = _request.getUri();
    if (parent.length() == 0)
        return false;

    // Remove trailing slash, if any.
    if (parent.size() > 1 && parent[parent.size() - 1] == '/')
        parent.erase(parent.size() - 1);

    // Find the parentdir path
    size_t found = parent.rfind("/");
    if (found > 0 && found != std::string::npos)
        parent.erase(found);
    else
        parent.erase(1);

    *(this->_stream) << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n";
    *(this->_stream) << "<html>\n<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" />\n";
    *(this->_stream) << "<head>\n<title>Index of " << _request.getUri() << "</title></head>\n";
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

        std::string file = "[   ] <a href=\"" + fullpath + "\">" + f->getFileName() + "</a> ";
        *(this->_stream) << std::setw(60) << std::left << file;
        *(this->_stream) << std::setw(24) << f->getModifDate()->getStr() << " ";
		if (f->isDirectory())
			*(this->_stream) << "-" << "\n";
		else
			*(this->_stream) << f->getSize() << "\n";
        ++it;
    }
    *(this->_stream) << "</pre><hr><address>ZiaHttpd Server at http://" << 
        this->_request.getHeaderOption("Host").substr(0, this->_request.getHeaderOption("Host").find(":")) << 
        this->_request.getUri() << " Port ";
    if (this->_request.getHeaderOption("Host").find(":") != std::string::npos)
        *(this->_stream) << this->_request.getHeaderOption("Host").substr(this->_request.getHeaderOption("Host").find(":") + 1);
    else
        *(this->_stream) << this->_request.getConfig()->getParam("Port");
    *(this->_stream) << "</address>\n</body></html>";
    return true;
}

