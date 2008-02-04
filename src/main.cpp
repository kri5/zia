#include <iostream>
#include "zia.h"
#include "Init.h"
#include "test.h"
#include "MainSocket.h"
#include "ClientSocket.h"
#include "Socket.h"

#include <libxml++/libxml++.h>

int main(int argc, char **argv)
{
    // Please remove this line when finished with the config parser.
	  xmlpp::DomParser	doc;

    Init i(argc, argv);
    return i.run();
}

