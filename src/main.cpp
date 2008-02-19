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
#include "Parser.h"

#include "MemoryManager.hpp"

int main(int argc, char **argv)
{
	Init		i(argc, argv);

	std::vector<IFile*>*	files;
	FileSystem		fs(".");
	files = fs.getFileList();
	std::vector<IFile*>::iterator it = files->begin();
	while (it != files->end())
	{
		std::cout << (*it)->getFileName() << " " << (*it)->getModifDate()->getStr() << std::endl;
		++it;
	}

	std::cout << std::boolalpha << fs.checkRights("zia.conf", FileSystem::Write) << std::endl;
	i.run();
	Logger::deleteInstance();
    
    //  For testing purpose (Parser)

    //std::string test("GET / HTTP/1.1\r\nContent-Length:5\r\n\r\n01234");
    //std::string out;
    //Parser *p = new Parser();
    //p->feed(test.c_str());
    //while (p->readIdentifier(out) == true)
    //    std::cout << out << std::endl;      
}

