#include "modphp.h"
#include <iostream>
#include <fstream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>

extern "C" zAPI::IModule* create()
{
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

extern "C" int  version()
{
    return 5;
}

ModPHP::ModPHP()
{

}

ModPHP::~ModPHP()
{

}

int                             ModPHP::getPriority(zAPI::IModule::Event event) const
{
    return 0;
}

zAPI::IModule::ChainStatus      ModPHP::onPreSend(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    int* fds_input = new int[2];
    int* fds_output = new int[2];
    
    request->setParam("modphp_fds_input", fds_input);
    request->setParam("modphp_fds_output", fds_output);

    pipe(fds_input);
    pipe(fds_output);
    
    pid_t pid = fork();
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

        char *newargv[] = { "php-cgi", NULL };
        char *newenviron[] = { "SERVER_SOFTWARE=ziahttpd",
                                "SERVER_NAME=tachatte",
                                "GATEWAY_INTERFACE=CGI/1.1",
                                "SERVER_PROTOCOL=HTTP/1.1",
                                "SERVER_PORT=8880",
                                "REQUEST_METHOD=GET",
                                //"PATH_INFO=",
                                "PATH_TRANSLATED=/home/etix/dev/zia/trunk/www/index.php",
                                "SCRIPT_NAME=/index.php",
                                "QUERY_STRING=/index.php",
                                "REMOTE_HOST=REMOTE_ADDR",
                                "REMOTE_ADDR=127.0.0.1",
                                "CONTENT_TYPE=HTTP",
                                "CONTENT_LENGTH=0",
                                NULL };
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
        close(fds_input[0]);
        char buf[1024];
        size_t len;
        while (len = response->getCurrentStream()->read(buf, 1024))
        {
            if (write(fds_input[1], buf, 1024) == -1)
                return zAPI::IModule::ERRORMODULE;
        }
        close(fds_input[1]);
    }
    return zAPI::IModule::CONTINUE;
}

size_t                          ModPHP::onProcessContent(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response, char* buf,
                                size_t size, std::vector<zAPI::ISendResponse*> tab, unsigned int index)
{
    int* fds_output = (int*)request->getParam("modphp_fds_output");
    if (tab.size() == index + 1)
    {
        size_t len = read(fds_output[0], buf, size);
        if (len < size)
            buf[len] = 0;
        return len;
    }
    else
    {
        std::cerr << "ModPHP must be the first module on the ProcessContent." << std::endl;
        return zAPI::IModule::ERRORMODULE;
    }
}

zAPI::IModule::ChainStatus      ModPHP::onPostSend(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    int* fds_output = (int*)request->getParam("modphp_fds_output");
    int* fds_input = (int*)request->getParam("modphp_fds_input");
    delete[] fds_output;
    delete[] fds_input;
}

zAPI::IModule::ChainStatus    ModPHP::onPreBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    response->setHeaderInStream(true);
    response->setHeaderOption("Transfer-Encoding", "chunked");
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus    ModPHP::onPostBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
{
    return zAPI::IModule::CONTINUE;
}


//zAPI::IModule::ChainStatus      ModPHP::onPostReceive(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response)
//{
//    std::cout << "Event: onPostReceive" << std::endl;
//    // PHP GOES HERE :D
//
//    int fds_input[2];
//    int fds_output[2];
//    pipe(fds_input);
//    pipe(fds_output);
//    pid_t pid = fork();
//    if (pid == -1)
//    {
//        std::cerr << "Fork failed, modphp can't continue." << std::endl;
//        return zAPI::IModule::CONTINUE;
//    }
//    else if (pid == 0)
//    {
//        // Child
//        char *newargv[] = { "php-cgi", NULL };
//        char *newenviron[] = { "SERVER_SOFTWARE=ziahttpd",
//                                "SERVER_NAME=tachatte",
//                                "GATEWAY_INTERFACE=CGI/1.1",
//                                "SERVER_PROTOCOL=HTTP/1.1",
//                                "SERVER_PORT=8880",
//                                "REQUEST_METHOD=GET",
//                                //"PATH_INFO=",
//                                "PATH_TRANSLATED=/home/etix/dev/zia/trunk/www/index.php",
//                                "SCRIPT_NAME=/index.php",
//                                "QUERY_STRING=/index.php",
//                                "REMOTE_HOST=REMOTE_ADDR",
//                                "REMOTE_ADDR=127.0.0.1",
//                                "CONTENT_TYPE=HTTP",
//                                "CONTENT_LENGTH=0",
//                                NULL };
//        dup2(0, fds_input[0]);
//        dup2(1, fds_output[1]);
//        if (execve("/usr/bin/php-cgi", newargv, newenviron) == -1)
//        {
//            perror("Execve");
//            exit(1);
//        }
//    }
//    else
//    {
//        // Parent
//
//        // Giving file to php
//        std::ifstream f("/home/etix/dev/zia/trunk/www/index.php", std::fstream::out);
//        
//        char bigbuf[10000];
//        f.read(bigbuf, 10000);
//        f.close();
//        write(fds_input[1], bigbuf, 10000);
//
//        // Reading php output
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

