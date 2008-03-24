#include "zia.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "ZException.hpp"
#include "Init.h"
#include "Sockets/IClientSocket.h"
#include "FileSystem/FileSystem.h"
#include "Time/ITime.h"
#include "File/File.h"
#include "Http/HttpParser.h"
#include "Server.h"
#include "Utils/Buffer.h"
#include "RootConfig.hpp"
#include "Threads/IThread.h"
#include "Workflow/Worker.h"
#include "Mutex/Mutex.h"
#include "Mutex/IMutex.h"
#include "MutexLock.hpp"

#include "MemoryManager.hpp"

int main(int argc, char **argv)
{
	Init		i(argc, argv);
	Server*		srv;

	if (i.start() == EXIT_FAILURE)
        return EXIT_FAILURE;
    try
    {
	    srv = new Server(i.getBindList(), i.getPool());
	}
	catch (ZException<Server>& ex)
    {
        Logger::getInstance() << Logger::Error << "Can't start server : " << ex.what() << Logger::Flush;
        return EXIT_FAILURE;
    }
	srv->run();

    delete srv;
	
	Logger::deleteInstance(); 
}

