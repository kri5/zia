#ifndef VHOST_H__
 #define VHOST_H__

#include <string>
#include <vector>

#include "NetworkID.h"
#include "Config.h"
#include "Mutex/IMutex.h"

/// Represent a Virtual Host. Mainly used for storing configuration.
class Vhost : public Config
{
	public:
		struct	Error
		{
			enum	Code
			{
				Unknown,
				InvalidConfig
			};
			static const char* Msg[];
		};
		Vhost(NetworkID*, Config*);
		virtual ~Vhost();
		/// Will return the address the VHost is binded to.
		const Address&		getAddress() const;
		/// Will return the port the VHost is binded to.
		const Port&			getPort() const;
		/// Will return the complete NetworkID :
		const NetworkID&	getNetworkID() const;
		/// Param getter overloading, to ensure that some parameters can't be getted (such as login or group for daemon mode)
		std::string			getParam(std::string) const;
		/// Will return the appropriate Vhost from the server's list, using Host: from Http header
		static const Config* getVhost(const std::vector<const Vhost*>& vhosts, const std::string& host);
	private:
        static IMutex*      _mutex;
        NetworkID*		    _netId;
};

#endif //VHOST_H__

