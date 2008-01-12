#include <iostream>
#include "zia.h"

#ifdef WIN32
 #include "zSocket_win32.h"
#else
 #include "zSocket_unix.h"
#endif


int main(int argc, char **argv)
{
	zSocket sock;
	sock.bind("127.0.0.1", 7171);
	sock.listen(5);
	sock.accept();
	return (0);
}