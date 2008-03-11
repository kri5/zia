#include "zia.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "ZException.hpp"
#include "Init.h"
#include "IClientSocket.h"
#include "FileSystem.h"
#include "ITime.h"
#include "File.h"
#include "HttpParser.h"
#include "Server.h"
#include "Date.h"
#include "Buffer.h"
#include "RootConfig.hpp"
#include "IThread.h"
#include "Worker.h"

#include "MemoryManager.hpp"

int main(int argc, char **argv)
{
	Init		i(argc, argv);
	Server*		srv;

	if (i.start() == EXIT_FAILURE)
        return EXIT_FAILURE;
    try
    {
	    srv = new Server(i.getBindList(), i.getRootConfig(), i.getPool());
	}
	catch (ZException<Server>& ex)
    {
        Logger::getInstance() << Logger::Error << "Can't start server : " << ex.what() << Logger::Flush;
        return EXIT_FAILURE;
    }
	srv->run();

    delete srv;
	
	Logger::deleteInstance(); 



//    Buffer      *buff = new Buffer(6);
//    
//    buff->push("0123456789abcdefghij012345678901234567890123456789", 50);
//    buff->clear();
//    std::cout << "<<<<" << std::endl;
//    buff->push("abcdef", 6);
//    std::cout << ">>>>" << std::endl;
//    char* line;
//    line = buff->get(6);
//    std::cout << line << std::endl;
//    delete[] line;
//    
//    //buff->flush(17);
//    
//    //line = buff->get(17);
//    //std::cout << line << std::endl;
//    //delete[] line;
//    buff->clear();
//    buff->dump();
//    delete buff;
}

