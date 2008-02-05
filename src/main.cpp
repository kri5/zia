#include <iostream>
#include "zia.h"
#include "Init.h"
#include "test.h"
#include "MainSocket.h"
#include "ClientSocket.h"
#include "Socket.h"


int main(int argc, char **argv)
{
	Init	i(argc, argv);
	i.run();
}

