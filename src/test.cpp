#include <iostream>
#include "test.h"

void		test::create(const char* text)
{
	(new test(text))->run();
}

void		test::code()
{
	std::cout << "Started thread #" << pid() << " for " << name.c_str() << std::endl;

	for (int i = 1; i <= 10; ++i)
	{
		sleep(1);
		std::cout << name.c_str() <<  " says: timeout in " << i << std::endl;
	}
}

