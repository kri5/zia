#ifndef ROOT_CONFIG_HPP__
# define ROOT_CONFIG_HPP__

#include "Singleton.hpp"
#include "Config.h"

class RootConfig : public Singleton<RootConfig>
{
	public:
		void		setConfig(const Config* cfg)
		{
			this->_cfg = const_cast<Config*>(cfg);
		}
		const Config*	getConfig() const
		{
			return this->_cfg;
		}
	private:
        RootConfig() : _cfg(NULL){}
		 Config*		_cfg;
	friend class Singleton<RootConfig>;
};

#endif //ROOT_CONFIG_HPP__
