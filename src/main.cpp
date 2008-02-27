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
#include "Buffer.h"

#include "MemoryManager.hpp"

int main(int argc, char **argv)
{
	Init		i(argc, argv);
	Server*		srv;

	i.start();
    try
    {
	    srv = new Server(i.getBindList());
    }
    catch (ZException<Server>& ex)
    {
        Logger::getInstance() << Logger::Error << "Can't start server : " << ex.what() << Logger::Flush;
        return EXIT_FAILURE;
    }
	srv->run();
	////
	//Logger::deleteInstance(); 
    //  For testing purpose (Parser)

	//std::string test("HEAD http://slashdot.org/test.php?empty=&t=42&foo=bar HTTP/1.1\r\n");
    //std::string test2("Content-Length: 42\r\n");
    //std::string test3("Host: test.ssh.t0mb.org:8000\r\n\r\n");
    //std::string out;
    //HttpParser *p = new HttpParser();
    //p->feed(test);
    //p->parse();
    //p->feed(test2);
    //p->feed(test3);
    //std::cout << "parse done == " << p->done() << std::endl;
    //p->parse();
    //std::cout << "parse done == " << p->done() << std::endl;
    //p->getRequest()->print();
    //std::string* dt = new std::string("Date: Tue, 12 Nov 1994 23:12:31 GMT");
    //Date* t = new Date(*dt);*/
//    Buffer      buff(6);
//
//    buff.push("abcdefghijk12345", 16);
//    buff.dump();
//    buff.get(5);
//    buff.dump();
}

