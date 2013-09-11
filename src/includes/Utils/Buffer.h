#ifndef BUFFER_H__
# define BUFFER_H__

#include <list>
#include <cstdlib>
#include <string>

class Buffer {
public:
  Buffer(size_t capacity);
  ~Buffer();

  bool hasEOL() const;
  /// Buff needs to be length + 1 char.
  void get(std::string &buff, size_t length);
  void push(const char *data, size_t size);
  void pushFront(const std::string &data);
  size_t capacity() const;
  size_t size() const;
  bool getLine(std::string &);
  void dump();
  void flush(size_t);
  void flush();
  void clear();
  size_t gcount() const;
  bool empty() const;
  char getChar(size_t);

private:
  size_t _capacity;
  size_t _size;
  int _eol;
  size_t _bufPos;
  std::list<std::string *> _buffers;
  size_t _readCount;

  void packBuffer(size_t);
  void getEolPos();
};

#endif //BUFFER_H__

