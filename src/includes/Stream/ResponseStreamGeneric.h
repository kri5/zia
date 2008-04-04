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
        Type                getType() const;
        size_t              getSize() const;
	private:
		std::stringstream*	_stream;
};

#endif // RESPONSESTREAMGENERIC_H__
