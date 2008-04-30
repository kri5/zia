#include "modphp.h"
#include <iostream>
#include <fstream>

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>

extern "C"  void    modphp_catchpipe(int sig)
{
    std::cout << "Broken pipe in php" << std::endl;
}

extern "C" zAPI::IModule* create()
{
    signal(SIGPIPE, modphp_catchpipe);
    //signal(SIGCHLD, SIG_IGN);
    return new ModPHP;
}

extern "C" void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "ModPHP";
}

extern "C" float version()
{
    return 5.0;
}

ModPHP::ModPHP()
{

}

ModPHP::~ModPHP()
{

}

int                             ModPHP::getPriority(zAPI::IModule::Event event) const
{
    if (event == zAPI::IModule::onPreBuildEvent)
        return 10;
    return 0;
}

char**                          ModPHP::createEnv(zAPI::IHttpRequest* req) const
{
    std::map<std::string, std::string>      env;
    char                                    port[42];
    sprintf(port, "%d", req->getClient()->getPort());

    env["SERVER_SOFTWARE"] = "ziahttpd";
    env["SERVER_NAME"] = "tachatte";
    env["HTTP_HOST"] = *(req->getConfig()->getParam("ServerName"));
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_PORT"] = port;
    env["REQUEST_METHOD"] = req->getCommand();
    env["SERVER_PROTOCOL"] = req->getProtocol();
    //env["PATH_INFO"] = req->getUri();
    env["PATH_TRANSLATED"] = *(req->getConfig()->getParam("DocumentRoot")) + req->getUri();
    env["SCRIPT_FILENAME"] = *(req->getConfig()->getParam("DocumentRoot")) + req->getUri();
    env["SCRIPT_NAME"] = req->getUri();
    env["QUERY_STRING"] = req->getUriQuery();
    env["REMOTE_HOST"] = "REMOTE_ADDR";
    env["REMOTE_ADDR"] = req->getClient()->getIP();
    env["CONTENT_TYPE"] = "HTTP";
    if (req->headerOptionIsSet("Content-Length"))
        env["CONTENT_LENGTH"] = req->getHeaderOption("Content-Length");
    else
        env["CONTENT_LENGTH"] = "0";
    if (req->headerOptionIsSet("Content-Type"))
        env["CONTENT_TYPE"] = req->getHeaderOption("Content-Type");

    char** newEnv = new char*[env.size() + 1];
    std::map<std::string, std::string>::const_iterator      it = env.begin();
    std::map<std::string, std::string>::const_iterator      ite = env.end();
    
    int     i;
    for (i = 0; it != ite; ++it, ++i)
        newEnv[i] = strdup(std::string(it->first + "=" + it->second).c_str());
    newEnv[i] = NULL;
    return newEnv;
}

zAPI::IModule::ChainStatus      ModPHP::onPreSend(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    if (request->getParam("modphp_status") != NULL)
    {
        int* fds_input = new int[2];
        int* fds_output = new int[2];

        request->setParam("modphp_fds_input", fds_input);
        request->setParam("modphp_fds_output", fds_output);

        if (pipe(fds_input) < 0)
            return zAPI::IModule::ERRORMODULE;
        if (pipe(fds_output) < 0)
            return zAPI::IModule::ERRORMODULE;

        pid_t   pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return zAPI::IModule::ERRORMODULE;
        }
        else if (pid == 0)
        {
            // Child
            dup2(fds_input[0], 0);
            dup2(fds_output[1], 1);
            close(fds_input[1]);
            close(fds_output[0]);

            char *newargv[] = { "php-cgi", NULL };
            char** newenviron = this->createEnv(request);
            if (execve("/usr/bin/php-cgi", newargv, newenviron) == -1)
            {
                perror("Execve");
                close(fds_output[0]);
                close(fds_output[1]);
                exit(1);
            }
        }
        else
        {
            request->setParam("modphp_pid_t", reinterpret_cast<void*>(pid));
            close(fds_output[1]);
            close(fds_input[0]);
            zAPI::IResponseStream* post = request->getBodyStream();
            if (post)
            {
                char    buff[1024];
                size_t  ret;
                while (post->completed() == false)
                {
                    ret = post->read(buff, 1024);
                    write(fds_input[1], buff, ret);
                }
            }
            close(fds_input[1]);
        }
    }
    return zAPI::IModule::CONTINUE;
}

size_t                          ModPHP::onProcessContent(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response, char* buf,
                                size_t size, const std::vector<zAPI::ISendResponse*>& tab, unsigned int index)
{
    if (request->getParam("modphp_status") != NULL)
    {
        pid_t   pid         = reinterpret_cast<pid_t>(request->getParam("modphp_pid_t"));
        int*    fds_output  = reinterpret_cast<int*>(request->getParam("modphp_fds_output"));
        int     *exited     = reinterpret_cast<int*>(request->getParam("modphp_exited"));
        int     status;

        waitpid(pid, &status, WNOHANG);
        if (!*exited && WIFEXITED(status))
            *exited = 1;

        int len = read(fds_output[0], buf, size);
        if (len < 0)
        {
            std::cerr << "Reading error in modphp: " << strerror(errno) << std::endl;
            return 0;
        }
        if (len < size)
            buf[len] = 0;

        ssize_t toRead = size - len;
        int     readA = len;
        int     nLen = 0;
        if (!*exited && len != size)
        {
            while (toRead > 0)
            {
                nLen = read(fds_output[0], &buf[readA], toRead);
                toRead -= nLen;
                readA += nLen;
                waitpid(pid, &status, WNOHANG);
                if (WIFEXITED(status))
                {
                    *exited = 1;
                    break;
                }
            }
            len = readA;
        }
        return (size_t)len;
    }
    size_t  ret;
    if (tab.size() == index + 1)
    {
        ret = response->getCurrentStream()->read(buf, size);
    }
    else
        ret = tab[index + 1]->onProcessContent(request, response, buf, size, tab, index + 1);
    return ret;
}

zAPI::IModule::ChainStatus      ModPHP::onPostSend(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    if (request->getParam("modphp_status") != NULL)
    {
        int* fds_output = reinterpret_cast<int*>(request->getParam("modphp_fds_output"));
        int* fds_input  = reinterpret_cast<int*>(request->getParam("modphp_fds_input"));
        int* exited     = reinterpret_cast<int*>(request->getParam("modphp_exited"));
        close(fds_output[0]);
        delete[] fds_output;
        delete[] fds_input;
        delete[] exited;
    }
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus    ModPHP::onPreBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    size_t  pos;
    int* exited = new int;
    *exited = 0;

    if ((pos = request->getUri().rfind('.')) != std::string::npos &&
            request->getUri().compare(pos, std::string::npos, ".php") == 0)
    {
        request->setParam("modphp_status", reinterpret_cast<void*>(1));
        request->setParam("modphp_exited", reinterpret_cast<void*>(exited));
        response->setHeaderInStream(true);
        request->setHeaderOption("Connection", "close");
        response->setHeaderOption("Connection", "close");
    }
    else
        request->setParam("modphp_status", NULL);
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus    ModPHP::onPostBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    return zAPI::IModule::CONTINUE;
}


