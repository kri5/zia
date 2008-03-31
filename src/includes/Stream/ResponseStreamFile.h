#ifndef RESPONSESTREAMFILE_H__
# define RESPONSESTREAMFILE_H__

#include <istream>

#include "API/IResponseStream.h"
#include "File/IFile.h"

class ResponseStreamFile : public IResponseStream
{
    public:
        ResponseStreamFile(IFile*);
        ~ResponseStreamFile();
        std::iostream&          getContent();
        bool                    completed() const;
        Type                    getType() const;
        size_t                  getSize() const;
        bool                    good() const;
    private:
        IFile*                  _file;
        Type                    _type;
};

#endif // RESPONSESTREAMFILE_H__

