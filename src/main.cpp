#include "zia.h"

#include <iostream>
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

	File		f("zia.conf");
	std::cout << f.getModifDate()->getStr() << " " << f.getSize() << std::endl;
	i.run();
	Logger::deleteInstance();
}

