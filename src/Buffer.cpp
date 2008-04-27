#include <string.h>
#include <string>
#include <iostream>

#include "Utils/Buffer.h"

#include "MemoryManager.hpp"

Buffer::Buffer(size_t capacity) : _capacity(capacity), _eol(-1), _bufPos(0)
{
    _size = 0;
}

Buffer::~Buffer()
{
    std::vector<std::string*>::iterator      it = this->_buffers.begin();
    std::vector<std::string*>::iterator      ite = this->_buffers.end();

    while (it != ite)
    {
        delete *it;
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
    this->_size += size;
    if (this->_buffers.size() == 0 || 
            this->_buffers.back()->length() + size > this->_capacity)
    {
        std::string* str = new std::string(data, size);
        this->_buffers.push_back(str);
        this->_bufPos = 0;
    }
    else
    {
        this->_buffers.back()->append(data, size);
        this->_bufPos += size;
    }
    if (this->hasEOL() == false)
        this->getEolPos();
}

void    Buffer::packBuffer(size_t begin)
{
    if (this->_buffers.size() == 0)
    {
        return ;
    }
    //std::cout << "last erase caracter : " << this->_buffers.front()->at(begin) << std::endl;
    this->_buffers.front()->erase(0, begin);
    this->_bufPos = 0;
    this->getEolPos();
}

void	Buffer::get(std::string& res, size_t length)
{
	//size_t                                  i = 0;
	size_t                                  nb = 0;
    std::vector<std::string*>::iterator     it = this->_buffers.begin();
    std::vector<std::string*>::iterator     ite = this->_buffers.end();
    res.clear();
    res.reserve(length);

	//for (; nb < length; ++nb)
	//{
	//	if (i == (*it)->length())
	//	{
	//		++it;
	//		if (it == ite)
	//		{
	//			this->_readCount = nb;
	//			res[nb] = 0;
	//			return ;
	//		}
	//		i = 0;
	//	}
	//	else if (nb == this->_size)
	//	{
	//		this->_readCount = nb;
	//		res[nb] = 0;
	//		return ;
	//	}
	//	res[nb] = (*it)->at(i);
	//	++i;
	//}
	//res[nb] = 0;
	//this->_readCount = nb;
    while (nb < length && it != ite)
    {
        if ((*it)->length() >= length)
        {
            res.append(*(*it), 0, length);
            nb += length;
            break ;
        }
        else
        {
            res.append(*(*it));
            length -= (*it)->length();
            nb += (*it)->length();
        }
        ++it;
    }
    this->_readCount = nb;
    //char* ptr = NULL;
    //ptr[2] = 0;
}

//char*   Buffer::get(size_t length)
//{
//    char* res = new char[length + 1];
//	this->get(res, length);
//	return res;
//}

char    Buffer::getChar(size_t pos)
{
	size_t                                  i = 0;
    std::vector<std::string*>::iterator       it = this->_buffers.begin();
    std::vector<std::string*>::iterator       end = this->_buffers.end();
    
    if (it == end)
        return -1;
    for (; it != end; ++it)
    {
        if (pos >= i + (*it)->length()) //resultat dans un buffer suivant.
        {
            i += (*it)->length();
            continue ;
        }
        else //le resultat est dans ce buffer.
        {
            if (pos >= i + (*it)->length())
            {
                //std::cout << "pos == " << pos << std::endl << " i == " << i << " length " << (*it)->length() << std::endl;
                //this->dump();
                return -1;
            }
            return ((*it)->at(pos - i));
        }
    }
    return -1;
}

void    Buffer::dump()
{
    std::vector<std::string*>::iterator       it;

    for (it = this->_buffers.begin(); it != this->_buffers.end(); ++it)
        std::cout << *(*it) << "]><[";
    std::cout << std::endl;
}

void        Buffer::getEolPos()
{
    std::vector<std::string*>::iterator       it = this->_buffers.begin();
    std::vector<std::string*>::iterator       ite = this->_buffers.end();
    //int                                     nb;
    //size_t                                  i;
    //bool                                    backslashAire = false;
    //size_t                                  length;

    //for (nb = 0; it != end; ++it)
    //{
    //    length = (*it)->length();
    //    for (i = 0; i < length; ++i, ++nb)
    //    {
    //        if (backslashAire == true)
    //        {
    //            if ((*it)->at(i) == '\n')
    //            {
    //                this->_eol = nb;
    //                return ;    
    //            }
    //            else
    //                backslashAire = false;
    //        }
    //        else
    //        {
    //            if ((*it)->at(i) == '\r')
    //            {
    //                backslashAire = true;
    //            }
    //        } 
    //    }
    //}
    //this->_eol = -1;
    size_t      pos;
    for (; it != ite; ++it)
    {
        if ((pos = (*it)->find("\r\n")) != std::string::npos)
        {
            this->_eol = pos;
            return ;
        }
    }
    this->_eol = -1;
}

void    Buffer::flush(size_t length)
{
    if (length == 0)
        return ;
    std::vector<std::string*>::iterator       it = this->_buffers.begin();
    std::vector<std::string*>::iterator       end = this->_buffers.end();
    size_t                                  nb;
    size_t                                  i;
    size_t                                  strLength;

    for (nb = 0; it != end && length > 0; )
    {
        strLength = (*it)->length();
        for (i = 0; i < strLength; ++i, ++nb)
        {
            if (i == strLength) //warning : bug generator
            {
                break ;
            }
            if (nb == length)
            {
                this->packBuffer(i);
                this->_size -= i;
                return ;
            }
        }
        delete *it;
        it = this->_buffers.erase(it);
        end = this->_buffers.end();
        this->_size -= strLength;
        if (nb == length - 1)
            return ;
        //if (length >= strLength)
        //{
        //    delete *it;
        //    it = this->_buffers.erase(it);
        //    length -= strLength;
        //    this->_size -= strLength;
        //    continue ;
        //}
        //else
        //{
        //    this->packBuffer(length);
        //    this->_size -= strLength;
        //    return ;
        //}
    }
    this->_size = 0;
}

void    Buffer::flush()
{
    this->flush(this->gcount());
}

void    Buffer::clear()
{
    std::vector<std::string*>::iterator      it = this->_buffers.begin();
    std::vector<std::string*>::iterator      end = this->_buffers.end();

    while (it != end)
    {
        delete *it;
        ++it;
    }
    this->_size = 0;
    this->_buffers.clear();
    this->_bufPos = 0;
    this->_eol = -1;
    _buffers.push_back(new std::string);
}

size_t  Buffer::gcount() const
{
    return this->_readCount;
}

bool    Buffer::empty() const
{
    return (this->_size == 0);
}

