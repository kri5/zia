#ifndef ISOCKET_H__
 #define ISOCKET_H__

#ifndef WIN32
	#include <poll.h>
#endif

class ISocket
{
	public:
		virtual ~ISocket(){}
		virtual void 			close(bool) = 0;
        virtual bool            isClosed() const = 0;
		virtual const ISocket&	operator>>(struct pollfd&) const = 0;
		virtual bool            isSet(const struct pollfd&) const = 0;
        virtual void            setPollFlag(int) = 0;
};

#endif //ISOCKET_H__
