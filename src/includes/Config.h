#ifndef CONFIG_H__
# define CONFIG_H__

#include <list>
#include <map>
#include <string>

#include "API/IConfig.h"

/// Stores the config values. To be used in clients and main sockets classes.
class Config : public zAPI::IConfig
{
	public:
		struct	Error
		{
			enum Code
			{
				Unknown,
				NotRootConfig
			};
			static const char*	Msg[];
		};
		Config();
		/// Copy constructor, to allow configuration inheritance.
		Config(const Config&);
		virtual ~Config();
		/// Will return the param value as a string. May be overide by inherited classes.
		const std::string*                      getParam(const std::string&) const;
        const char*                             getParamChar(const std::string&) const;
		/// Used to set a param, while parsing XML file.
		void									setParam(const std::string&, const std::string&);

		void									addMimeType(std::string ext, std::string type);
		std::string								getMimeType(std::string ext) const;
        void                                    addModule(std::string location);
        const std::list<std::string>&           getModules() const;
        bool                                    isSet(const std::string& name) const;
        void                                    dump() const;
	protected:
		/// Default port values :
		enum DefaultPort
		{
			_DefaultPort = 80,
			_SslPort = 443
		};
		/// Will return the default port, depending on UseSSL value.
        void        removeParameter(std::string name);
		/// The param list, stocked as name => value
		std::map<std::string, std::string>		_params;
		std::map<std::string, std::string>*		_mime;
        std::list<std::string>*                 _modules;
		bool									_globalConf;
};

#endif //CONFIG_H__

