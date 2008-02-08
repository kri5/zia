#ifndef MEMORYMANAGER_H__
# define MEMORYMANAGER_H__

#ifndef NDEBUG

#include "zia.h"

#include <map>

#include "Singleton.hpp"
#include "Logger.hpp"

/// Will track memory leaks. Automaticly enabled if NDEBUG isn't set, and if the file is included at the top of the file. Must be last file included.
class MemoryManager
{
	private:
		struct	MemoryBlock
		{
			int				size;
			int				line;
			std::string		file;
			bool			isArray;
		};

	public:
		/// Sort of stack singleton.
		static MemoryManager&	getInstance()
		{
			static MemoryManager	m;
		
			return m;
		}

		/// used for blocks allocation.
		void*	alloc(std::size_t size, const char* file, int line, bool isArray)
		{
			void*	ptr = malloc(size);
			if (ptr == NULL)
				throw std::bad_alloc();
		
			MemoryBlock 	newBlock;
			newBlock.size = size;
			newBlock.line = line;
			newBlock.file = file;
			newBlock.isArray = isArray;
			this->_blocks[ptr] = newBlock;
		
			//Logger::getInstance() << Logger::DEBUGMODE << "Allocation of " << size << " bytes, from " << file << " at line " << line << Logger::FLUSH;
			return ptr;
		}

		/// Will free a block, since delete is overloaded.
		void	free(void* ptr, bool isArray)
		{
			std::map<void*, MemoryBlock>::iterator		it;
		
			it = this->_blocks.find(ptr);
			if (it == this->_blocks.end())
			{
				::free(ptr);
				return ;
			}
			if (it->second.isArray != isArray)
			{
				throw 0;
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
			if (this->_blocks.empty())
			{
				Logger::getInstance() << Logger::DEBUGMODE << "No memory leak detected." << Logger::FLUSH;
			}
			else
			{
				Logger::getInstance() << Logger::ERRORLVL << "Warning : memory leaks detected :";

				std::map<void*, MemoryBlock>::iterator	it;
				it = this->_blocks.begin();
				while (it != this->_blocks.end())
				{
					//error msg.
					Logger::getInstance() << Logger::ERRORLVL << Zia::Newline << " - On " << it->second.file << " at line " << it->second.line
											<< " undeleted block of " << it->second.size << " bytes";
					::free(it->first);
					++it;
				}
				Logger::getInstance() << Logger::FLUSH;
				this->_blocks.clear();
			}
		}
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
inline void		operator delete(void* ptr)
{
	MemoryManager::getInstance().free(ptr, false);
}

/// Debug overloading of delete[]
inline void		operator delete[](void* ptr)
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

