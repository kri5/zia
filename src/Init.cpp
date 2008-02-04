#include "Init.h"

Init::Init(int argc, char **argv) : _argc(argc), _argv(argv)
{

}

Init::~Init()
{

}

/// Run the sequential initialization
int         Init::run()
{
    this->readCommandLine();
    this->readConfiguration();
    this->initSSL();
    this->initSockets();
    this->initThreads();

    return EXIT_SUCCESS; // EXIT_FAILURE on error
}

/// Read and process arguments from the command line
void        Init::readCommandLine()
{

}

/// Read the XML configuration
void        Init::readConfiguration()
{

}

/// Initialize the SSL features
void        Init::initSSL()
{
  SSL_library_init(); // load encryption & hash algorithms for SSL
  SSL_load_error_strings(); // load the error strings for good error reporting
  Logger::getInstance()->log(Logger::INFO, "SSL loaded successfully.");
}

/// Start the server sockets
void        Init::initSockets()
{

}

/// Spawn the threads
void        Init::initThreads()
{

}

