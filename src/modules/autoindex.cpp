#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/types.h>

#include "autoindex.h"


extern "C" zAPI::IModule* create()
{
    return new AutoIndex;
}

extern "C" void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "AutoIndex module";
}

extern "C" float  version()
{
    return 1.0;
}

int         AutoIndex::getPriority(zAPI::IModule::Event event) const
{
    return 0;
}

zAPI::IModule::ChainStatus    AutoIndex::onPreBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    std::string fileName = request->getConfig()->getParamChar("DocumentRoot") + request->getUri();
    struct stat st;
    if (stat(fileName.c_str(), &st) < 0)
        return IModule::CONTINUE;
    if (S_ISDIR(st.st_mode))
    {
        DIR*			dir = opendir(fileName.c_str());
        struct dirent*	file;

        if (!dir)
        {
            closedir(dir);
            return IModule::CONTINUE;
        }
        while ((file = readdir(dir)))
        {
            if (file->d_name[0] == '.' &&
                    (file->d_name[1] == 0 || (file->d_name[1] == '.' && file->d_name[2] == 0)))
                continue ;
            if (strcmp(file->d_name, "index.php") == 0)
            {
                request->setUri(request->getUri() + "/index.php");
                std::cout << "after autoindex : " << request->getUri() << std::endl;
                return IModule::CONTINUE;
            }
        }
    }
    return IModule::CONTINUE;
}

zAPI::IModule::ChainStatus    AutoIndex::onPostBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    return IModule::CONTINUE;
}

