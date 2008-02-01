#include <iostream>
#include "zia.h"
#include "test.h"
#include "MainSocket.h"
#include "ClientSocket.h"
#include "XMLDocument.h"
#include "XMLNode.h"
#include "Socket.h"


int main(int argc, char **argv)
{

/*      Socket part       */
/*	char buf[16];

	zSocket sock;
	sock.bind("127.0.0.1", 7001);
	sock.listen(5);

	zSocket *newSock = sock.accept();
	newSock->recv(buf, 16);
	newSock->send("It worked!\n", 11);
	newSock->recv(buf, 16);
	newSock->close(true);
	sock.close(true);
	delete newSock;
*/


/*      Thread part     */
	/*Mutex mutex;
	test::create("Moulaf", mutex);
	test::create("roolz", mutex);
	
	sleep(1);
	while (!mutex.trylock())
		sleep(1);
	mutex.unlock();
*/

	/* XML Part */

	Xml::Document* doc = new Xml::Document("zia.conf");
	Xml::Node* root = doc->getRootNode();
	std::cout << "<" << root->getName() << ">" << std::endl;
	Xml::Node*	subNode;
	while (subNode = root->getNextChildNode())
	{
		std::cout << '<' << subNode->getName() << '>' << subNode->getValue() 
			<< "</" << subNode->getName() << '>' << std::endl;
	}
	std::cout << "</" << root->getName() << ">" << std::endl;

	return (0);
}

