#ifndef VHOST_H__
 #define VHOST_H__

#include <string>

#include "NetworkID.h"
#include "Config.h"

/// Represent a Virtual Host. Mainly used for store configuration.
class Vhost : public Config
{
	public:
		Vhost(NetworkID*);
		/// Will return the address the VHost is binded to.
		Address*		getAddress() const;
		/// Will return the port the VHost is binded to.
		Port*			getPort() const;

		/// Param getter overloading, to ensure that some parameters can't be getted (such as login or group for daemon mode)
		std::string		getParam(std::string) const;
	private:
		/// Private constructor, so you can only instantiate via the factory method.
		NetworkID*		_netId;
};

#endif //VHOST_H__

