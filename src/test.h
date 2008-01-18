#ifndef __TEST_H__
#define __TEST_H__

#include "IThread.h"

class		test : private IThread
{
public:
	static void create(const char* text);

private:
	test(const char* text) : name(text) {}
	virtual void code();
	std::string name;
};

#endif // __TEST_H__
