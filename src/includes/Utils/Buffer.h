#ifndef BUFFER_H__
# define BUFFER_H__

#include <list>
#include <stdlib.h>

class Buffer
{
    public:
        Buffer(size_t size);
        ~Buffer();

        bool        hasEOL() const;
        char*       getLine();
        char*       get(size_t length);
		/// Buff needs to be length + 1 char.
		void		get(char* buff, size_t length);
        void        push(const char* data, size_t size);
        size_t      capacity() const;
        size_t      size() const;
        void        dump();
        void        flush(size_t);
        void        flush();
        void        clear();
        size_t      gcount() const;
        bool        empty() const;
        char        getChar(size_t);
    private:
        size_t              _capacity;
        size_t              _size;
        int                 _eol;
        size_t              _bufPos;
        std::list<char*>    _buffers;
        size_t              _readCount;

        void                packBuffer(size_t);
        void                getEolPos();
};

#endif //BUFFER_H__

