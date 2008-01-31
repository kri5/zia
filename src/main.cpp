#include <iostream>
#include "zia.h"
#include "test.h"
#include "MainSocket.h"
#include "ClientSocket.h"
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

	MainSocket		*sock;
	char			buff[16];

	sock = new MainSocket(5, "127.0.0.1", 7001);
	ClientSocket *clt = sock->accept();
	clt->send("It worked\n", 11);
	delete clt;
	delete sock;

/*      Thread part     */
	Mutex mutex;
	test::create("Moulaf", mutex);
	test::create("roolz", mutex);
	
	sleep(1);
	while (!mutex.trylock())
		sleep(1);
	mutex.unlock();

	return (0);
}

