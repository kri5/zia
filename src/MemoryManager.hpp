#ifndef MEMORYMANAGER_H__
# define MEMORYMANAGER_H__

#define NDEBUG
#ifndef NDEBUG

#include <map>
#include <stack>

#include "zia.h"
#include "Singleton.hpp"
#include "Logger.hpp"
#include "Mutex.h"
#include "MutexLock.hpp"

/// Will track memory leaks. Automaticly enabled if NDEBUG isn't set, and if the file is included at the top of the file. Must be last file included.
class MemoryManager
{
	private:
		struct	MemoryBlock
		{
			int				size;
			unsigned int	line;
			std::string		file;
			bool			isArray;
		};

	public:
		static MemoryManager&		getInstance()
		{
			static MemoryManager	mem;

			return mem;
		}

		/// used for blocks allocation.
		void*	alloc(std::size_t size, const char* file, int line, bool isArray)
		{
            MutexLock(&(this->_mutex));
			void*	ptr = malloc(size);
			if (ptr == NULL)
				throw std::bad_alloc();

			MemoryBlock 	newBlock;
			newBlock.size = size;
			newBlock.line = line;
			newBlock.file = file;
			newBlock.isArray = isArray;
			this->_blocks[ptr] = newBlock;

			//Logger::getInstance() << Logger::PrintStdOut << Logger::Debug << "Allocation of " << size << " bytes, from " << file << " at line " << line << Logger::Flush;
			return ptr;
		}

		/// Will free a block, since delete is overloaded.
		void	free(void* ptr, bool isArray)
		{
            MutexLock(&(this->_mutex));
			std::map<void*, MemoryBlock>::iterator		it;

			it = this->_blocks.find(ptr);
			if (it == this->_blocks.end())
			{
				//::free(ptr);
				return ;
			}
			if (it->second.isArray != isArray)
			{
				throw std::bad_alloc();
			}
			::free(it->first);
			this->_blocks.erase(it);
		}

	private:
		std::map<void*, MemoryBlock>	_blocks;
		MemoryManager()
		{
		}
		~MemoryManager()
		{
            MutexLock(&(this->_mutex));
			if (this->_blocks.empty())
			{
				std::cout << "No memory leak detected." << std::endl;
			}
			else
			{
				Logger::getInstance() << Logger::Error << "Warning : memory leaks detected :";

				std::map<void*, MemoryBlock>::iterator	it;
				it = this->_blocks.begin();
				while (it != this->_blocks.end())
				{
					//error msg.
					std::cerr << Zia::Newline << " - On " << it->second.file << " at line " << it->second.line
						<< " undeleted block of " << it->second.size << " bytes\n";
					++it;
				}
				std::cout << std::endl;
				this->_blocks.clear();
			}
		}
        Mutex   _mutex;
};

/// Debug overloading of new
inline void*	operator new(std::size_t size, const char* file, int line)
{
	return MemoryManager::getInstance().alloc(size, file, line, false);
}

/// Debug overloading of new[]
inline void*	operator new[](std::size_t size, const char* file, int line)
{
	return MemoryManager::getInstance().alloc(size, file, line, true);
}

/// Debug overloading of delete
inline void		operator delete(void* ptr) throw()
{
	MemoryManager::getInstance().free(ptr, false);
}

/// Debug overloading of delete[]
inline void		operator delete[](void* ptr) throw()
{
	MemoryManager::getInstance().free(ptr, true);
}

/// delete overload, to be called if new (IE the CTOR) throws an exception.
inline void		operator delete(void* ptr, const char*, int)
{
	MemoryManager::getInstance().free(ptr, false);
}

/// delete[] overload, to be called if new[] (IE one of the CTOR called) throws an exception
inline void		operator delete[](void* ptr, const char*, int)
{
	MemoryManager::getInstance().free(ptr, true);
}

/// Convenience define, since you don't want to type new (__FILE__, __LINE__) for every allocation ;)
#define new new(__FILE__, __LINE__)

#endif //NDEBUG

#endif //MEMORYMANAGER_H__

