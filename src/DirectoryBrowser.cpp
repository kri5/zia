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
    std::list<IFile*>::iterator       it = _fileList->begin();
    std::list<IFile*>::iterator       end = _fileList->end();

    while (it != end)
    {
        delete *it;
        ++it;
    }
    this->_fileList->clear();
}

HttpResponse&           DirectoryBrowser::getResponse()
{
   HttpResponse* response = new HttpResponse();
   response->setResponseStatus(200);

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

   std::stringstream* content = new std::stringstream;
   *content << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n";
   *content << "<html>\n<head>\n<title>Index of " << _request.getUri() << "</title></head>\n";
   *content << "<body><h1>Index of " << _request.getUri() << "</h1>\n";
   *content << "<pre>Name                    Last modified      Size  Description<hr>\n";
   *content << "<a href=\"" << parent << "\">Parent Directory</a>                            -\n";
   
   std::list<IFile*>::iterator      it = this->_fileList->begin();
   std::list<IFile*>::iterator      ite = this->_fileList->end();

   while (it != ite)
   {
       std::string path;
       if (_request.getUri() != "/")
           path = _request.getUri() + "/";
       IFile* f = (*it);
       *content << "[   ]<a href=\"" << path + f->getFileName() << "\">" << f->getFileName() << "</a>     ";
       *content << f->getModifDate()->getStr() << "   ";
       *content << f->getSize() << "  \n";
       ++it;
   }
   
   *content << "<hr></pre><address>ZiaHttpd Server at _HOST_ Port _PORT_</address>\n</body></html>";

   std::istream* is = new std::istream(content->rdbuf());
   response->setContent(is);
   response->setContentLength(content->str().size());
    //FIXME : "is" can't be deleted.
   //delete is;
   return *response;
}

