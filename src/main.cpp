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
#include "RootConfig.hpp"

#include "MemoryManager.hpp"

int main(int argc, char **argv)
{
	Init		i(argc, argv);
	Server*		srv;

	if (i.start() == EXIT_FAILURE)
        return EXIT_FAILURE;
    try
    {
	    srv = new Server(i.getBindList(), i.getRootConfig());
	}
	catch (ZException<Server>& ex)
    {
        Logger::getInstance() << Logger::Error << "Can't start server : " << ex.what() << Logger::Flush;
        return EXIT_FAILURE;
    }
	srv->run();

    delete srv;
	////
	//Logger::deleteInstance(); 
    //  For testing purpose (Parser)

	//std::string test("HEAD http://slashdot.org/test.php?empty=&t=42&foo=bar HTTP/1.1\n");
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
//    Buffer      *buff = new Buffer(6);
//
//    buff->push("abcdefghijk12345", 16);
//    buff->push("0123456789abcdefghij012345678901234567890123456789", 50);
//    char* line;
//    line = buff->get(17);
//    std::cout << line << std::endl;
//    delete[] line;
//
//    buff->flush(17);
//
//    line = buff->get(17);
//    std::cout << line << std::endl;
//    delete[] line;
//    buff->clear();
//    buff->dump();
//    delete buff;
//    //buff.dump();
}

