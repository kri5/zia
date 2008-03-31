#ifndef ROOT_CONFIG_HPP__
# define ROOT_CONFIG_HPP__

#include "Singleton.hpp"
#include "Config.h"
#include <iostream>
class RootConfig : public Singleton<RootConfig>
{
    public:
        void		setConfig(Config* cfg)
        {
            this->_cfg = cfg;
        }
        static Config*	getConfig()
        {
            return RootConfig::getInstance()._cfg;
        }
        static std::string  getParam(std::string name)
        {
            return RootConfig::getConfig()->getParam(name);
        }
    private:
        RootConfig() : _cfg(NULL){}
        Config*		_cfg;
        friend class Singleton<RootConfig>;
};

#endif //ROOT_CONFIG_HPP__
