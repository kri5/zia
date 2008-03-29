#include "zia.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "ZException.hpp"
#include "Init.h"
#include "Server.h"
#include "Utils/Buffer.h"

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
//    Buffer      buff(5);
//
//    buff.push("abcdefghijklmnopqrtsuvwxyz", 26);
//    buff.flush(12);
//    std::cout << buff.get(9);
//    buff.dump();
}

