#include "zia.h"

#include <iostream>
#include <cstdlib>

#include "ZException.hpp"
#include "Init.h"
#include "IClientSocket.h"
#include "FileSystem.h"
#include "Time.h"

#include "MemoryManager.hpp"

int main(int argc, char **argv)
{
	Init		i(argc, argv);

	Time*	time = new Time();
	std::cout << time->getStr() << std::endl;
	i.run();
	Logger::deleteInstance();
}

