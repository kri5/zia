#include "zia.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "FileSystem/IFileSystem.h"
#include "FileSystem/FileSystem.h"
#include "ZException.hpp"
#include "Init.h"
#include "Server.h"

#include "MemoryManager.hpp"

int main(int argc, char **argv)
{
	//IFileSystem* fs = new FileSystem(".");
	//fs->getFileList(".conf");
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
	
	//Logger::deleteInstance(); 
    //Buffer      buff(5);

    //buff.push("abcdefghijklmnopqrtsuvwxyz", 26);
    //buff.dump();
    //buff.flush(12);
    //buff.dump();
    //std::cout << buff.get(14) << std::endl;
    //buff.dump();
    //buff.push("bite", 4);
    //buff.dump();
    //buff.flush();
    //buff.dump();
    //std::cout << buff.size() << std::endl;
}

