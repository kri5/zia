#include <string.h>
#include <string>
#include <iostream>

#include "Utils/Buffer.h"

#include "MemoryManager.hpp"

Buffer::Buffer(size_t capacity) : _capacity(capacity), _eol(-1), _bufPos(0)
{
    _buffers.push_back(new char[capacity]);
    _size = 0;
}

Buffer::~Buffer()
{
    std::list<char*>::iterator      it = this->_buffers.begin();
    std::list<char*>::iterator      ite = this->_buffers.end();

    while (it != ite)
    {
        delete[] *it;
        ++it;
    }
    this->_buffers.clear();
}

bool    Buffer::hasEOL() const
{
    return this->_eol >= 0;
}

size_t  Buffer::capacity() const
{
    return this->_capacity;
}

size_t  Buffer::size() const
{
    return this->_size;
}

void    Buffer::push(const char* data, size_t size)
{
    int  dataPos = 0;
    int  bakSize = size;

    this->_size += size;
    while (this->_bufPos + size > this->_capacity)
    {
        memcpy(this->_buffers.back() + this->_bufPos,
                data + dataPos,
                this->_capacity - this->_bufPos);
        dataPos += this->_capacity - this->_bufPos;
        this->_buffers.push_back(new char[this->_capacity]);
        size -= this->_capacity - this->_bufPos;
        this->_bufPos = 0;
    }
    memcpy(this->_buffers.back() + this->_bufPos,
            data + dataPos, bakSize - dataPos);
    this->_bufPos += size;
    if (this->hasEOL() == false)
        this->getEolPos();
}

void    Buffer::packBuffer(size_t begin)
{
    std::list<char*>::iterator      it = this->_buffers.begin();
    std::list<char*>::iterator      end = this->_buffers.end();
    std::list<char*>::iterator      next;

    if (begin == 0)
    {
        delete[] this->_buffers.front();
        this->_buffers.erase(this->_buffers.begin());
    }
    else
    {
        while (it != end)
        {
            next = it;
            ++next;
            memcpy((*it), &(*it)[begin], this->_capacity - begin);
            if (next != end)
            {
                memcpy(&(*it)[this->_capacity - begin], (*next), begin);
            }
            else
            {
                if (this->_bufPos < begin)
                {
                    delete[] *it;
                    this->_buffers.erase(it);
                    this->_bufPos += this->_capacity - begin;
                    break ;
                }
                else if (this->_bufPos == begin)
                {
                    this->_bufPos = 0;
                }
                else
                {
                    memset(&(*it)[this->_bufPos - begin], 0, this->_capacity - (this->_bufPos - begin));
                }
            }
            ++it;
        }
    }
    this->getEolPos();
}

void	Buffer::get(char* res, size_t length)
{
	size_t  i;
	size_t  nb;
	std::list<char*>::iterator      it = this->_buffers.begin();
	std::list<char*>::iterator      end = this->_buffers.end();

	i = 0;
	for (nb = 0; nb < length; ++nb)
	{
		if (i == this->_capacity)
		{
			++it;
			if (it == end)
			{
				this->_readCount = nb;
				res[nb] = 0;
				return ;
			}
			i = 0;
		}
		else if (nb == this->_size)
		{
			this->_readCount = nb;
			res[nb] = 0;
			return ;
		}
		res[nb] = (*it)[i];
		++i;
	}
	res[nb] = 0;
	this->_readCount = nb;
}

char*   Buffer::get(size_t length)
{
    char* res = new char[length + 1];
	this->get(res, length);
	return res;
}

void    Buffer::dump()
{
    std::list<char*>::iterator      it;
    int                             i = 0;

    for (it = this->_buffers.begin(); it != this->_buffers.end(); ++it)
    {
        std::cout << "#" << i << ">|" << std::string(*it, this->_capacity) << "|<" << std::endl;
        ++i;
    }
}

void        Buffer::getEolPos()
{
    std::list<char*>::iterator      it = this->_buffers.begin();
    std::list<char*>::iterator      end = this->_buffers.end();
    int                             nb;
    size_t                          i;
    bool                            backslashAire = false;

    for (nb = 0; it != end; ++it)
    {
        for (i = 0; i < this->_capacity; ++i, ++nb)
        {
            if (i == this->_capacity)
                break ;
            if (backslashAire == true)
            {
                if ((*it)[i] == '\n')
                {
                    this->_eol = nb;
                    return ;    
                }
                else
                    backslashAire = false;
            }
            else
            {
                if ((*it)[i] == '\r')
                {
                    backslashAire = true;
                }
            } 
        }
    }
    this->_eol = -1;
}

char*   Buffer::getLine()
{
    if (this->hasEOL())
    {
        return this->get(this->_eol + 1);
    }
    return NULL;
}

void    Buffer::flush(size_t length)
{
    std::list<char*>::iterator      it = this->_buffers.begin();
    std::list<char*>::iterator      end = this->_buffers.end();
    size_t                          nb;
    size_t                          i;

    for (nb = 0; it != end; )
    {
        for (i = 0; i < this->_capacity; ++i, ++nb)
        {
            if (nb == length)
            {
                this->packBuffer(i);
                this->_size -= length;
                return ;
            }
            if (i == this->_capacity)
                break ;
        }
        delete[] *it;
        it = this->_buffers.erase(it);
    }
    this->_size = 0;
    this->packBuffer(i);
}

void    Buffer::flush()
{
    this->flush(this->gcount());
}

void    Buffer::clear()
{
    std::list<char*>::iterator      it = this->_buffers.begin();
    std::list<char*>::iterator      end = this->_buffers.end();

    while (it != end)
    {
        delete[] *it;
        ++it;
    }
    this->_buffers.clear();
    this->_bufPos = 0;
    this->_eol = -1;
    _buffers.push_back(new char[this->_capacity]);
    memset(*(this->_buffers.begin()), 0, this->_capacity);
}

size_t  Buffer::gcount() const
{
    return this->_readCount;
}

bool    Buffer::empty() const
{
    return (this->_size == 0);
}
