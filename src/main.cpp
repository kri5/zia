#include "zia.h"

#include <iostream>
#include <iostream>

#include "ZException.h"
#include "MemoryManager.hpp"
#include "Init.h"
#include "IClientSocket.h"

int main(int argc, char **argv)
{
	Logger::getInstance() << Logger::Info 
        << "Kikoolol" << Logger::Flush;
	//Init		i(argc, argv);

    try
    {
        throw ZException<IClientSocket>
            (INFO, IClientSocket::Error::Send);
    }
    catch (ZException<IClientSocket> &e)
    {
        std::cout << e.what() << std::endl;
    }
}

