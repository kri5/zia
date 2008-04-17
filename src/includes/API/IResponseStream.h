#ifndef IRESPONSESTREAM_H__
# define IRESPONSESTREAM_H__

#include <istream>

namespace zAPI
{
    /// Base interface for response stream. This will encapsulate a stringstream or a filestream
    class   IResponseStream
    {
        public:
            /**
             *  The interface virtual destructor
             */
            virtual ~IResponseStream(){}

            virtual size_t              read(char* buff, size_t size) = 0;
            /**
             *  Returns wheter we are a the end of the stream or not
             *  @return stream position status
             */
            virtual bool                completed() const = 0;

            /**
             *  Returns the value of the good method of the underlying stream
             *  @return whether the stream is good for i/o operations or not
             */
            virtual bool                good() const = 0;

            /**
             *  Returns the size of the stream (the number of caracters that you can read)
             *  @return the size of the stream
             */ 
            virtual size_t              getSize() const = 0;
    };
}

#endif //IRESPONSESTREAM_H__

