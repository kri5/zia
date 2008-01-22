#include <iostream>
#include "zia.h"
#include "test.h"

#ifdef WIN32
 #include "zSocket_win32.h"
#else
 #include "zSocket_unix.h"
#endif


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
	Mutex mutex;
	test::create("Moulaf", mutex);
	sleep(1);
	test::create("roolz", mutex);
	
	sleep(1);
	while (!mutex.trylock())
		sleep(1);
	mutex.unlock();

	return (0);
}

