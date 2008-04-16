#ifndef RESPONSESTREAMFILE_H__
# define RESPONSESTREAMFILE_H__

#include <istream>

#include "API/IResponseStream.h"
#include "File/IFile.h"

class ResponseStreamFile : public zAPI::IResponseStream
{
    public:
        ResponseStreamFile(IFile*);
        ~ResponseStreamFile();
        std::iostream&          getContent();
        bool                    completed() const;
        size_t                  getSize() const;
        bool                    good() const;
    private:
        IFile*                  _file;
};

#endif // RESPONSESTREAMFILE_H__

