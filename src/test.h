#ifndef __TEST_H__
#define __TEST_H__

#include "zia.h"

#ifdef WIN32
 #include "IThread_win32.h"
 #include "Mutex_win32.h"
#else
 #include <unistd.h>
 #include "IThread_unix.h"
 #include "Mutex_unix.h"
#endif

class		test : private IThread
{
public:
	static void create(const char* text, Mutex& mutex);

private:
	test(const char* text, Mutex& _mutex) : name(text), mutex(_mutex) {}
	virtual void code();
	std::string name;
	Mutex& mutex;

};

#endif // __TEST_H__
