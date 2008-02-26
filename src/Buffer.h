#ifndef BUFFER_H__
# define BUFFER_H__

#include <list>
#include <stdlib.h>

class Buffer
{
    public:
        Buffer(size_t size);

        bool        hasEOL() const;
        char*       getLine();
        char*       get(size_t length);
        void        push(char* data, size_t size);
        size_t      getSize() const;
        void        dump();
    private:
        size_t              _size;
        int                 _eol;
        size_t              _bufPos;
        std::list<char*>    _buffers;

        void                packBuffer(size_t);
        void                getEolPos();
};

#endif //BUFFER_H__

