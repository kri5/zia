#ifndef RESPONSESTREAMGENERIC_H__
# define RESPONSESTREAMGENERIC_H__

#include <fstream>
#include <sstream>

#include "API/IResponseStream.h"

class	ResponseStreamGeneric : public zAPI::IResponseStream
{
	public:
		ResponseStreamGeneric(std::stringstream*);
		virtual ~ResponseStreamGeneric();

        std::iostream&      getContent();
        bool                completed() const;
        bool                good() const;
        size_t              getSize() const;
        size_t              read(char*, size_t);
	private:
		std::stringstream*	_stream;
};

#endif // RESPONSESTREAMGENERIC_H__
