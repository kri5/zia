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
    std::cout << "Broken pipe in php" << std::endl;
}

extern "C" zAPI::IModule* create()
{
    signal(SIGPIPE, modPerl_catchpipe);
    signal(SIGCHLD, SIG_IGN);
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
    return 1.0;
}

ModPerl::ModPerl()
{
    _perlInterpreter = perl_alloc();
    PERL_SET_CONTEXT(_perlInterpreter);
    perl_construct(_perlInterpreter);

}

void    ModPerl::init()
{
}

ModPerl::~ModPerl()
{
    PERL_SET_CONTEXT(this->_perlInterpreter);
    perl_destruct(this->_perlInterpreter);
    PERL_SET_CONTEXT(this->_perlInterpreter);
    perl_free(this->_perlInterpreter);
}

int                             ModPerl::getPriority(zAPI::IModule::Event event) const
{
    if (event == zAPI::IModule::onPreBuildEvent)
        return 10;
    return 1;
}

zAPI::IModule::ChainStatus      ModPerl::onPreSend(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    if (request->getParam("modPerl_status") != NULL)
    {
        char        *perlOpts[] = {"", "-e", "0"};
        struct stat sb;
        int         fd;
        void*       addr;

        PERL_SET_CONTEXT(this->_perlInterpreter);
        perl_parse(this->_perlInterpreter, NULL, 3, perlOpts, NULL);
        PERL_SET_CONTEXT(this->_perlInterpreter);
        perl_run(this->_perlInterpreter);
        fd = open((*(request->getConfig()->getParam("DocumentRoot")) + request->getUri()).c_str() ,O_RDONLY);
        if (fd == -1)
            return zAPI::IModule::ERRORMODULE;
        if (fstat(fd, &sb) == -1)
            return zAPI::IModule::ERRORMODULE;
        addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (addr == MAP_FAILED)
            return zAPI::IModule::ERRORMODULE; 

        ////FIXME: pipe sortie standard

        PERL_SET_CONTEXT(this->_perlInterpreter);
        perl_eval_pv((char*)addr,TRUE);
        //SV* val = perl_get_sv("main::ret", FALSE);
        //printf("%s\n", SvPV(val, PL_na));
        munmap(addr, sb.st_size);
    }


    //if (request->getParam("modphp_status") != NULL)
    //{
    //    int* fds_input = new int[2];
    //    int* fds_output = new int[2];

    //    request->setParam("modphp_fds_input", fds_input);
    //    request->setParam("modphp_fds_output", fds_output);

    //    if (pipe(fds_input) < 0)
    //        return zAPI::IModule::ERRORMODULE;
    //    if (pipe(fds_output) < 0)
    //        return zAPI::IModule::ERRORMODULE;

    //    pid_t   pid = fork();
    //    if (pid == -1)
    //    {
    //        perror("fork");
    //        return zAPI::IModule::ERRORMODULE;
    //    }
    //    else if (pid == 0)
    //    {
    //        // Child
    //        dup2(fds_input[0], 0);
    //        dup2(fds_output[1], 1);
    //        close(fds_input[1]);
    //        close(fds_output[0]);

    //        char *newargv[] = { "php-cgi", NULL };
    //        char** newenviron = this->createEnv(request);
    //        if (execve("/usr/bin/php-cgi", newargv, newenviron) == -1)
    //        {
    //            perror("Execve");
    //            close(fds_output[0]);
    //            close(fds_output[1]);
    //            exit(1);
    //        }
    //    }
    //    else
    //    {
    //        request->setParam("modphp_pid_t", reinterpret_cast<void*>(pid));
    //        close(fds_output[1]);
    //        close(fds_input[0]);
    //        zAPI::IResponseStream* post = request->getBodyStream();
    //        if (post)
    //        {
    //            char    buff[1024];
    //            size_t  ret;
    //            while (post->completed() == false)
    //            {
    //                ret = post->read(buff, 1024);
    //                write(fds_input[1], buff, ret);
    //            }
    //        }
    //        close(fds_input[1]);
    //    }
    //}
    return zAPI::IModule::CONTINUE;
}

size_t                          ModPerl::onProcessContent(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response, char* buf,
        size_t size, const std::vector<zAPI::ISendResponse*>& tab, unsigned int index)
{
    if (request->getParam("modPerl_ret") != NULL)
    {
        //if (tab.size() == index + 1)
        //{
        //    int len = read(fds_output[0], buf, size);
        //    if (len < 0)
        //    {
        //        std::cerr << "Reading error in modPerl: " << strerror(errno) << std::endl;
        //        return 0;
        //    }
        //    if (len < size)
        //        buf[len] = 0;
        //    return len;
        //}
        //else
        //{
        //    std::cerr << "ModPerl must be the first module on the ProcessContent." << std::endl;
        //    return zAPI::IModule::ERRORMODULE;
        //}
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
    //if (request->getParam("modphp_status") != NULL)
    //{
    //    int* fds_output = (int*)request->getParam("modphp_fds_output");
    //    int* fds_input = (int*)request->getParam("modphp_fds_input");
    //    close(fds_output[0]);
    //    delete[] fds_output;
    //    delete[] fds_input;
    //    int status;
    //    //pid_t pid = reinterpret_cast<pid_t>(request->getParam("modphp_pid_t"));
    //    //std::cout << "done : status == " << status << " " << waitpid(pid, &status, 0) << "done " << std::endl;
    //    //std::cout << WIFSTOPPED(status) << std::endl;
    //}
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus    ModPerl::onPreBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    size_t  pos;
    if ((pos = request->getUri().rfind('.')) != std::string::npos &&
            request->getUri().compare(pos, std::string::npos, ".pl") == 0)
    {
        request->setParam("modPerl_status", (void*)1);
        response->setHeaderInStream(false);
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

