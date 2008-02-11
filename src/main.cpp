#include "zia.h"

#include <iostream>
#include <cstdlib>

#include "ZException.hpp"
#include "MemoryManager.hpp"
#include "Init.h"
#include "IClientSocket.h"

#include "AtExit.hpp"

int main(int argc, char **argv)
{
	Init	i(argc, argv);
	
	i.run();
}

