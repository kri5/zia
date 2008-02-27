#include "DirectoryBrowser.h"

DirectoryBrowser::DirectoryBrowser(HttpRequest& request) : _request(request)
{
    // Read directory path from request
    // If it isn't a directory, throw 500
    _fs = new FileSystem(request.getConfig()->getParam("DocumentRoot") + request.getUri());
    _fileList = _fs->getFileList();
}

DirectoryBrowser::~DirectoryBrowser()
{

}

HttpResponse&           DirectoryBrowser::getResponse()
{
   HttpResponse* response = new HttpResponse();
   response->setResponseStatus(200);

   std::stringstream* content = new std::stringstream;
   *content << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n";
   *content << "<html>\n<head>\n<title>Index of " << _request.getUri() << "</title></head>\n";
   *content << "<body><h1>Index of " << _request.getUri() << "</h1>\n";
   *content << "<pre>Name                    Last modified      Size  Description<hr>\n";
   *content << "<a href=\"/\">Parent Directory</a>                            -\n";
   
   for (unsigned int i = 0; i < _fileList->size(); ++i)
   {
       IFile* f = (*_fileList)[i];
       *content << "[   ]<a href=\"" << _request.getUri() + "/" + f->getFileName() << "\">" << f->getFileName() << "</a>     ";
       *content << "19-Feb-2008 21:54   ";
       *content << "61K  \n";
   }
   
   *content << "<hr></pre><address>Apache Server at _HOST_ Port _PORT_</address>\n</body></html>";

   std::istream* is = new std::istream(content->rdbuf());
   response->setContent(is);
   response->setContentLength(content->str().size());
   return *response;
}

