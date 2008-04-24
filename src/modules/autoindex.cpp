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
    if (request->getConfig()->isSet("AutoIndex") == false)
        return IModule::CONTINUE;
    const std::string& autoIndex = *(request->getConfig()->getParam("AutoIndex"));
    std::string fileName = request->getConfig()->getParamChar("DocumentRoot") + request->getUri();
    struct stat st;

    if (stat(fileName.c_str(), &st) < 0)
        return IModule::CONTINUE;
    if (S_ISDIR(st.st_mode))
    {
        size_t	begin = 0;
        size_t	end;
        std::string     indexToSearch;
        while ((end = autoIndex.find(' ', begin)) != std::string::npos)
        {
            indexToSearch = autoIndex.substr(begin, end - begin);
            DIR*			dir = opendir(fileName.c_str());
            struct dirent*	file;

            if (!dir)
                return IModule::CONTINUE;
            while ((file = readdir(dir)))
            {
                if (file->d_name[0] == '.' &&
                        (file->d_name[1] == 0 || (file->d_name[1] == '.' && file->d_name[2] == 0)))
                    continue ;
                {
                    if (indexToSearch == file->d_name)
                    {
                        request->setUri(request->getUri() + indexToSearch);
                        closedir(dir);
                        return IModule::CONTINUE;
                    }
                }
                indexToSearch = autoIndex.substr(begin, end - begin);
                if (indexToSearch == file->d_name)
                {
                    request->setUri(request->getUri() + indexToSearch);
                    closedir(dir);
                    return IModule::CONTINUE;
                }
            }
            begin = end + 1;
            closedir(dir);
        }
    }
    return IModule::CONTINUE;
}

zAPI::IModule::ChainStatus    AutoIndex::onPostBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    return IModule::CONTINUE;
}

