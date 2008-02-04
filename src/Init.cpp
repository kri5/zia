#include "Init.h"

Init::Init(int argc, char **argv) : _argc(argc), _argv(argv)
{

}

Init::~Init()
{

}

int         Init::run()
{
    this->readCommandLine();
    this->readConfiguration();
    this->initSSL();
    this->initSockets();
    this->initThreads();

    return EXIT_SUCCESS; // EXIT_FAILURE on error
}

void        Init::readCommandLine()
{

}

void        Init::readConfiguration()
{

}

void        Init::initSSL()
{
  SSL_library_init(); // load encryption & hash algorithms for SSL
  SSL_load_error_strings(); // load the error strings for good error reporting
  Logger::getInstance()->log(Logger::INFO, "SSL loaded successfully.");
}

void        Init::initSockets()
{

}

void        Init::initThreads()
{

}

