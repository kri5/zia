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
		std::cout << (*it)->getFileName() << std::endl;
		++it;
	}

	std::cout << std::boolalpha << fs.checkRights("zia.conf", FileSystem::Write) << std::endl;
	i.run();
	Logger::deleteInstance();
}

