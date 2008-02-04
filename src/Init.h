#ifndef __INIT_H__
#define __INIT_H__

#include "zia.h"
#include "Logger.hpp"
#include <openssl/ssl.h>

class Init
{
    public:
        Init(int argc, char **argv);
        ~Init();

        int     run();
        void    readCommandLine();
        void    readConfiguration();
        void    initSSL();
        void    initSockets();
        void    initThreads();

    private:
        int     _argc;
        char**  _argv;  
};

#endif // __INIT_H__
