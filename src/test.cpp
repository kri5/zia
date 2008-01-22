#include <iostream>
#include "test.h"

void		test::create(const char* text, Mutex& mutex)
{
	(new test(text, mutex))->run();
}

void		test::code()
{
	mutex.lock();
	std::cout << "Started thread #" << pid() << " for " << name.c_str() << std::endl;

	for (int i = 1; i <= 10; ++i)
	{
		sleep(1);
		std::cout << name.c_str() <<  " says: timeout in " << i << std::endl;
	}
	mutex.unlock();
}

