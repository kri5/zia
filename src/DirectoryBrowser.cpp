#include "DirectoryBrowser.h"
#include "Utils/UrlString.h"
#include <iostream>
#include <iomanip>

DirectoryBrowser::DirectoryBrowser(const HttpRequest &request,
                                   std::iostream *stream)
    : _request(request), _first(true), _stream(stream) {
  // Read directory path from request
  // DocumentRoot has to be defined. This is normally checked in Init
  _fs = new FileSystem(*(request.getConfig()->getParam("DocumentRoot")) +
                       request.getUri());
  _fileList = _fs->getFileList();
}

DirectoryBrowser::~DirectoryBrowser() { delete this->_fs; }

void DirectoryBrowser::outputHeader()
{
  *(this->_stream)
      << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n";
  *(this->_stream) << "<html>\n<meta http-equiv=\"content-type\" "
                      "content=\"text/html; charset=UTF-8\" />\n";
  *(this->_stream) << "<head>\n<title>Index of " << _request.getUri()
                   << "</title></head>\n";
  *(this->_stream) << "<body><h1>Index of " << _request.getUri() << "</h1>\n";
  *(this->_stream) << "<pre>Name                    Last modified      Size  "
                      "Description<hr>\n";
  *(this->_stream) << "<a href=\"" << parent

}

void DirectoryBrowser::outputFooter()
{
  *(this->_stream) << "</pre><hr><address>ZiaHttpd Server at http://"
                   << this->_request.getHeaderOption("Host").substr(
                          0, this->_request.getHeaderOption("Host").find(":"))
                   << this->_request.getUri() << " Port ";
  if (this->_request.getHeaderOption("Host").find(":") != std::string::npos)
    *(this->_stream)
        << this->_request.getHeaderOption("Host")
               .substr(this->_request.getHeaderOption("Host").find(":") + 1);
  else
    *(this->_stream) << this->_request.getConfig()->getParam("Port");
  *(this->_stream) << "</address>\n</body></html>";
}

bool DirectoryBrowser::get() {
  if (this->_fileList == NULL)
    return false;
  std::string parent = _request.getUri();

  // Remove trailing slash, if any.
  if (parent.size() > 1) {
    if (parent[parent.size() - 1] == '/')
      parent.erase(parent.size() - 1);

    // Find the parentdir path
    size_t found = parent.rfind("/");
    if (found > 0)
      parent.erase(found);
    else
      parent.erase(1);
  }

  this->outputHeader();

  for (auto file : this->_fileList) {
   std::string path;
   if (_request.getUri() != "/")
     path = _request.getUri() + "/";

   std::string fullpath = path + file->getFileName();
   fullpath = UrlString::urlEncode(fullpath);
   *(this->_stream)
       << std::setw(60) << std::left
       << "[   ] <a href=\"" + fullpath + "\">" + file->getFileName() + "</a> ";
   *(this->_stream) << std::setw(24) << file->getModifDate()->getStr() << " ";
   if (file->isDirectory())
     *(this->_stream) << "-\n";
   else
     *(this->_stream) << file->getSize() << "\n";
  }

  this->outputFooter();

  return true;
}

