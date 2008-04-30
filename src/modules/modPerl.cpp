#include "modPerl.h"
#include <iostream>
#include <fstream>

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>

extern "C"  void    modPerl_catchpipe(int sig)
{
    std::cout << "Broken pipe in Perl" << std::endl;
}

extern "C" zAPI::IModule* create()
{
    signal(SIGPIPE, modPerl_catchpipe);
    //signal(SIGCHLD, SIG_IGN);
    return new ModPerl;
}

extern "C" void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "ModPerl";
}

extern "C" float version()
{
    return 5.0;
}

ModPerl::ModPerl()
{

}

ModPerl::~ModPerl()
{

}

int                             ModPerl::getPriority(zAPI::IModule::Event event) const
{
    if (event == zAPI::IModule::onPreBuildEvent)
        return 10;
    return 0;
}

//char**                          ModPerl::createEnv(zAPI::IHttpRequest* req) const
//{
//    std::map<std::string, std::string>      env;
//    char                                    port[42];
//    sprintf(port, "%d", req->getClient()->getPort());
//
//    env["SERVER_SOFTWARE"] = "ziahttpd";
//    env["SERVER_NAME"] = "tachatte";
//    env["HTTP_HOST"] = *(req->getConfig()->getParam("ServerName"));
//    env["GATEWAY_INTERFACE"] = "CGI/1.1";
//    env["SERVER_PROTOCOL"] = "HTTP/1.1";
//    env["SERVER_PORT"] = port;
//    env["REQUEST_METHOD"] = req->getCommand();
//    env["SERVER_PROTOCOL"] = req->getProtocol();
//    //env["PATH_INFO"] = req->getUri();
//    env["PATH_TRANSLATED"] = *(req->getConfig()->getParam("DocumentRoot")) + req->getUri();
//    env["SCRIPT_FILENAME"] = *(req->getConfig()->getParam("DocumentRoot")) + req->getUri();
//    env["SCRIPT_NAME"] = req->getUri();
//    env["QUERY_STRING"] = req->getUriQuery();
//    env["REMOTE_HOST"] = "REMOTE_ADDR";
//    env["REMOTE_ADDR"] = req->getClient()->getIP();
//    env["CONTENT_TYPE"] = "HTTP";
//    if (req->headerOptionIsSet("Content-Length"))
//        env["CONTENT_LENGTH"] = req->getHeaderOption("Content-Length");
//    else
//        env["CONTENT_LENGTH"] = "0";
//    if (req->headerOptionIsSet("Content-Type"))
//        env["CONTENT_TYPE"] = req->getHeaderOption("Content-Type");
//
//    char** newEnv = new char*[env.size() + 1];
//    std::map<std::string, std::string>::const_iterator      it = env.begin();
//    std::map<std::string, std::string>::const_iterator      ite = env.end();
//    
//    int     i;
//    for (i = 0; it != ite; ++it, ++i)
//        newEnv[i] = strdup(std::string(it->first + "=" + it->second).c_str());
//    newEnv[i] = NULL;
//    return newEnv;
//}

zAPI::IModule::ChainStatus      ModPerl::onPreSend(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    if (request->getParam("modPerl_status") != NULL)
    {
        int* fds_input = new int[2];
        int* fds_output = new int[2];

        request->setParam("modPerl_fds_input", fds_input);
        request->setParam("modPerl_fds_output", fds_output);

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

            char        *perlOpts[] = {"", "-e", "0"};
            struct stat sb;
            int         fd;
            void*       addr;
            PerlInterpreter* _perl;
            _perl = perl_alloc();
            perl_construct(_perl);
            perl_parse(_perl, NULL, 3, perlOpts, NULL);
            perl_run(_perl);
            fd = open((*(request->getConfig()->getParam("DocumentRoot")) + request->getUri()).c_str() ,O_RDONLY);
            if (fd == -1)
                exit(1);
            if (fstat(fd, &sb) == -1)
            {
                close(fd);
                exit(1);
            }
            addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (addr == MAP_FAILED)
            {
                close(fd);
                exit(1);
            }
            perl_eval_pv((char*)addr,TRUE);
            //SV* val = perl_get_sv("main::ret", FALSE);
            //printf("%s\n", SvPV(val, PL_na));
            munmap(addr, sb.st_size);
            close(fd);
            perl_destruct(_perl);
            perl_free(_perl);
            exit(1);
            //char *newargv[] = { "Perl-cgi", NULL };
            //char** newenviron = this->createEnv(request);
            //if (execve("/usr/bin/Perl-cgi", newargv, newenviron) == -1)
            //{
            //    perror("Execve");
            //    close(fds_output[0]);
            //    close(fds_output[1]);
            //    exit(1);
            //}
        }
        else
        {
            request->setParam("modPerl_pid_t", reinterpret_cast<void*>(pid));
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

size_t                          ModPerl::onProcessContent(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response, char* buf,
                                size_t size, const std::vector<zAPI::ISendResponse*>& tab, unsigned int index)
{
    if (request->getParam("modPerl_status") != NULL)
    {
        pid_t   pid = reinterpret_cast<pid_t>(request->getParam("modPerl_pid_t"));
        int*    fds_output = (int*)request->getParam("modPerl_fds_output");
        int     *exited = (int*)(request->getParam("modPerl_exited"));
        int     status;

        waitpid(pid, &status, WNOHANG);
        if (!*exited && WIFEXITED(status))
            *exited = 1;

        if (tab.size() == index + 1)
        {
            int len = read(fds_output[0], buf, size);
            if (len < 0)
            {
                std::cerr << "Reading error in modPerl: " << strerror(errno) << std::endl;
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
        else
        {
            std::cerr << "ModPerl must be the first module on the ProcessContent." << std::endl;
            return zAPI::IModule::ERRORMODULE;
        }
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

zAPI::IModule::ChainStatus      ModPerl::onPostSend(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    if (request->getParam("modPerl_status") != NULL)
    {
        int* fds_output = (int*)request->getParam("modPerl_fds_output");
        int* fds_input = (int*)request->getParam("modPerl_fds_input");
        int* exited = (int*)request->getParam("modPerl_exited");
        close(fds_output[0]);
        delete[] fds_output;
        delete[] fds_input;
        delete[] exited;
    }
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus    ModPerl::onPreBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    size_t  pos;
    int* exited = new int;
    *exited = 0;

    if ((pos = request->getUri().rfind('.')) != std::string::npos &&
            request->getUri().compare(pos, std::string::npos, ".pl") == 0)
    {
        request->setParam("modPerl_status", (void*)1);
        request->setParam("modPerl_exited", (void*)exited);
        response->setHeaderInStream(true);
        request->setHeaderOption("Connection", "close");
        response->setHeaderOption("Connection", "close");
    }
    else
        request->setParam("modPerl_status", NULL);
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus    ModPerl::onPostBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    return zAPI::IModule::CONTINUE;
}


//zAPI::IModule::ChainStatus      ModPerl::onPostReceive(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
//{
//    std::cout << "Event: onPostReceive" << std::endl;
//    // PERL GOES HERE :D
//
//    int fds_input[2];
//    int fds_output[2];
//    pipe(fds_input);
//    pipe(fds_output);
//    pid_t pid = fork();
//    if (pid == -1)
//    {
//        std::cerr << "Fork failed, modPerl can't continue." << std::endl;
//        return zAPI::IModule::CONTINUE;
//    }
//    else if (pid == 0)
//    {
//        // Child
//        char *newargv[] = { "Perl-cgi", NULL };
//        char *newenviron[] = { "SERVER_SOFTWARE=ziahttpd",
//                                "SERVER_NAME=tachatte",
//                                "GATEWAY_INTERFACE=CGI/1.1",
//                                "SERVER_PROTOCOL=HTTP/1.1",
//                                "SERVER_PORT=8880",
//                                "REQUEST_METHOD=GET",
//                                //"PATH_INFO=",
//                                "PATH_TRANSLATED=/home/etix/dev/zia/trunk/www/index.Perl",
//                                "SCRIPT_NAME=/index.Perl",
//                                "QUERY_STRING=/index.Perl",
//                                "REMOTE_HOST=REMOTE_ADDR",
//                                "REMOTE_ADDR=127.0.0.1",
//                                "CONTENT_TYPE=HTTP",
//                                "CONTENT_LENGTH=0",
//                                NULL };
//        dup2(0, fds_input[0]);
//        dup2(1, fds_output[1]);
//        if (execve("/usr/bin/Perl-cgi", newargv, newenviron) == -1)
//        {
//            perror("Execve");
//            exit(1);
//        }
//    }
//    else
//    {
//        // Parent
//
//        // Giving file to Perl
//        std::ifstream f("/home/etix/dev/zia/trunk/www/index.Perl", std::fstream::out);
//        
//        char bigbuf[10000];
//        f.read(bigbuf, 10000);
//        f.close();
//        write(fds_input[1], bigbuf, 10000);
//
//        // Reading Perl output
//        ssize_t len;
//        char buf[1024 + 1];
//        while ((len = read(fds_output[0], buf, 1024)) != 0)
//        {
//           buf[len] = '\0'; 
//           std::cout << buf << std::endl;
//        }
//        std::cout << "Goodbye" << std::endl;
//    }
//    return zAPI::IModule::CONTINUE;
//}

