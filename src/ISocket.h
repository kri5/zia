#ifndef __ISOCKET_HPP__
#define __ISOCKET_HPP__

#include "zia.h"

/// Socket interface to plugin OS dependant code.
class ISocket
{
public:
	//virtual int select() const = 0;
	/// Close the connection with auto-shutdown support.
	virtual void close(bool shutdown) const = 0;
};


#endif // __ISOCKET_HPP__

