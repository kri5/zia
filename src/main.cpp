#include "zia.h"

#include <iostream>
#include <iostream>

#include "ZException.h"
#include "MemoryManager.hpp"
#include "Init.h"

int main(int argc, char **argv)
{
	Init		i(argc, argv);
	Logger::getInstance() << Logger::INFO << "Kikoolol";
}

