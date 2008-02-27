#include <string.h>
#include <string>
#include <iostream>

#include "Buffer.h"

Buffer::Buffer(size_t size) : _size(size), _eol(-1), _bufPos(0)
{
    _buffers.push_back(new char[size]);
}

bool    Buffer::hasEOL() const
{
    return this->_eol >= 0;
}

size_t  Buffer::getSize() const
{
    return this->_size;
}

void    Buffer::push(char* data, size_t size)
{
    int  dataPos = 0;
    int  bakSize = size;

    while (this->_bufPos + size > this->_size)
    {
        memcpy(this->_buffers.back() + this->_bufPos,
                data + dataPos,
                this->_size - this->_bufPos);
        dataPos += this->_size - this->_bufPos;
        this->_buffers.push_back(new char[this->_size]);
        size -= this->_size - this->_bufPos;
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
            memcpy((*it), &(*it)[begin], this->_size - begin);
            if (next != end)
            {
                memcpy(&(*it)[this->_size - begin], (*next), begin);
            }
            else
            {
                if (this->_bufPos < begin)
                {
                    delete[] *it;
                    this->_buffers.erase(it);
                    this->_bufPos += this->_size - begin;
                    break ;
                }
                else if (this->_bufPos == begin)
                {
                    this->_bufPos = 0;
                }
                else
                {
                    memset(&(*it)[this->_bufPos - begin], 0, this->_size - (this->_bufPos - begin));
                }
            }
            ++it;
        }
    }
    this->getEolPos();
}

char*   Buffer::get(size_t length)
{
    size_t  i;
    size_t  nb;
    char*   res;
    std::list<char*>::iterator      it = this->_buffers.begin();
    std::list<char*>::iterator      end = this->_buffers.end();

    res = new char[length + 1];
    i = 0;
    for (nb = 0; nb < length; ++nb)
    {
        if (i == this->_size)
        {
            delete[] *it;
            it = this->_buffers.erase(it);
            end = this->_buffers.end();
            if (it == end)
            {
                res[nb] = 0;
                this->_eol = -1;
                return res;
            }
            i = 0;
        }
        res[nb] = (*it)[i];
        ++i;
    }
    res[nb] = 0;
    this->packBuffer(i % this->_size);
    return res;
}

void    Buffer::dump()
{
    std::list<char*>::iterator      it;
    int                             i = 0;

    for (it = this->_buffers.begin(); it != this->_buffers.end(); ++it)
    {
        std::cout << "#" << i << " " << std::string(*it, this->_size) << '|' << std::endl;
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
        for (i = 0; i < this->_size; ++i, ++nb)
        {
            if (i == this->_size)
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

