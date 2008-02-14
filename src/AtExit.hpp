#ifndef ATEXIT_HPP__
# define ATEXIT_HPP__

#include <iostream>

/*
 * /!\ DO NOT INCLUDE THIS ANYWHERE ELSE THAN MAIN.CPP !! /!\
 */

/// This class will automaticly destroy both Logger and MemoryManager.
class	AtExit
{
	public:
		~AtExit()
		{
#ifndef NDEBUG
			MemoryManager::deleteInstance();
#endif //NDEBUG
			Logger::deleteInstance();
		}
};

AtExit	__AtExit;

#endif //DEBUG_HPP__
