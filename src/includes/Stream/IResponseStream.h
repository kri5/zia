#ifndef IRESPONSESTREAM_H__
# define IRESPONSESTREAM_H__

#include <istream>

class   IResponseStream
{
    public:
        enum    Type
        {
            FileStream,
            DirStream,
            ErrorStream
        };
        virtual ~IResponseStream(){};
        virtual std::iostream&      getContent() = 0;
        virtual bool                completed() const = 0;
        virtual Type                getType() const = 0;
        virtual size_t              getSize() const = 0;
};

#endif //IRESPONSESTREAM_H__

