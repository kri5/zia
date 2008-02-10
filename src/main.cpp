#include "zia.h"

#include <iostream>
#include <iostream>

#include "ZException.h"
#include "MemoryManager.hpp"
#include "Init.h"
#include "IClientSocket.h"

int main(int argc, char **argv)
{
	Init		i(argc, argv);
	i.run();
	Logger::deleteInstance();
}

