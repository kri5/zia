#include <string.h>
#include <string>
#include <iostream>

#include "Utils/Buffer.h"

#include "MemoryManager.hpp"

Buffer::Buffer(size_t capacity)
    : _capacity(capacity), _eol(-1), _bufPos(0), _size(0) {}

Buffer::~Buffer() {
  for (auto buffer : this->_buffers)
    delete buffer;
  this->_buffers.clear();
}

bool Buffer::hasEOL() const { return this->_eol >= 0; }

size_t Buffer::capacity() const { return this->_capacity; }

size_t Buffer::size() const { return this->_size; }

void Buffer::push(const char *data, size_t size) {
  this->_size += size;
  if (this->_buffers.size() == 0 ||
      this->_buffers.back()->length() + size > this->_capacity) {
    std::string *str = new std::string(data, size);
    this->_buffers.push_back(str);
    this->_bufPos = 0;
  } else {
    this->_buffers.back()->append(data, size);
    this->_bufPos += size;
  }
  if (this->hasEOL() == false)
    this->getEolPos();
}

void Buffer::pushFront(const std::string &buff) {
  this->_size += buff.size();
  this->_buffers.push_front(new std::string(buff));
  this->getEolPos();
}

void Buffer::packBuffer(size_t begin) {
  if (this->_buffers.size() == 0) {
    return;
  }
  //std::cout << "last erase caracter : " << this->_buffers.front()->at(begin)
  //<< std::endl;
  this->_buffers.front()->erase(0, begin);
  this->_bufPos = 0;
  this->getEolPos();
}

void Buffer::get(std::string &res, size_t length) {
  //size_t                                  i;
  size_t nb = 0;
  auto it = this->_buffers.begin();
  auto ite = this->_buffers.end();

  res.clear();
  res.reserve(length);
  while (nb < length && it != ite) {
    if ((*it)->length() >= length) {
      res.append(*(*it), 0, length);
      nb += length;
      break;
    } else {
      res.append(*(*it));
      length -= (*it)->length();
      nb += (*it)->length();
    }
    ++it;
  }
  this->_readCount = nb;
}

char Buffer::getChar(size_t pos) {
  size_t i = 0;

  for (auto buffer : this->_buffers) {
    if (pos >= i + buffer->length())
      i += buffer->length();
    else
      return buffer->at(pos - i);
  }
  return -1;
}

void Buffer::dump() {
  for (auto buffer : this->_buffers)
    std::cout << buffer << "]><[";
  std::cout << std::endl;
}

void Buffer::getEolPos() {
  size_t pos;

  for (auto buffer : this->_buffers) {
   if ((pos = buffer->find("\r\n")) != std::string::npos) {
     this->_eol = pos;
     return;
   }
  }
  this->_eol = -1;
}

bool Buffer::getLine(std::string &res) {
  if (this->hasEOL()) {
    this->get(res, this->_eol + 2);
    return true;
  }
  return false;
}

void Buffer::flush(size_t length) {
  if (length == 0)
    return;
  auto it = this->_buffers.begin();
  auto end = this->_buffers.end();
  size_t nb;
  size_t i;
  size_t strLength;

  for (nb = 0; it != end && length > 0;) {
    strLength = (*it)->length();
    for (i = 0; i < strLength; ++i, ++nb) {
      if (i == strLength) {
        break;
      }
      if (nb == length) {
        this->packBuffer(i);
        this->_size -= i;
        return;
      }
    }
    delete *it;
    it = this->_buffers.erase(it);
    this->_size -= strLength;
    if (nb == length - 1)
      return;
  }
  this->_size = 0;
}

void Buffer::flush() { this->flush(this->gcount()); }

void Buffer::clear() {
  for (auto buffer : this->_buffers)
	  delete buffer;
  this->_size = 0;
  this->_buffers.clear();
  this->_bufPos = 0;
  this->_eol = -1;
  _buffers.push_back(new std::string);
}

size_t Buffer::gcount() const { return this->_readCount; }

bool Buffer::empty() const { return (this->_size == 0); }

