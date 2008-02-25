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
#include "HttpParser.h"
#include "Server.h"
#include "Date.h"

#include "MemoryManager.hpp"

int main(int argc, char **argv)
{
	Init		i(argc, argv);
	Server*		srv;

	i.start();
	srv = new Server(i.getBindList());

	srv->run();
	//
	//Logger::deleteInstance(); 
    //  For testing purpose (Parser)

	/*std::string test("GET http://slashdot.org/test.php?empty=&t=4=2&foor=bar HTTP/1.1\r\nContent-Length: 42\r\nHost: localhost:8880\r\nFrom: thomas.boquet@gmail.com\r\nDate: Tue, 12 Nov 1994 23:12:31 GMT\r\n\r\n01234");
    std::string out;
    HttpParser *p = new HttpParser();
    p->feed(test);
    p->parse();
    while (p->readIdentifier(out) == true)
        std::cout << "->" << out << std::endl; */     
    //std::string* dt = new std::string("Date: Tue, 12 Nov 1994 23:12:31 GMT");
    //Date* t = new Date(*dt);
}

