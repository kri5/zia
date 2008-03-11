#ifndef CONFIG_H__
# define CONFIG_H__

#include <vector>
#include <map>
#include <string>

/// Stores the config values. To be used in clients and main sockets classes.
class Config
{
	public:
		Config();
		/// Copy constructor, to allow configuration inheritance.
		Config(const Config&);
		virtual ~Config();
		/// Will return the param value as a string. May be overide by inherited classes.
		virtual std::string						getParam(std::string) const;
		/// Used to set a param, while parsing XML file.
		void									setParam(std::string, std::string);

		void									addMimeType(std::string ext, std::string type);
		std::string								getMimeType(std::string ext) const;
        bool                                    isSet(std::string name) const;
        void                                    dump() const;
	protected:
		/// Default port values :
		enum DefaultPort
		{
			_DefaultPort = 80,
			_SslPort = 443
		};
		/// Will return the default port, depending on UseSSL value.
		int			getDefaultPort() const;
        void        removeParameter(std::string name);
		/// The param list, stocked as name => value
		std::map<std::string, std::string>		_params;
		std::map<std::string, std::string>		_mime;
		bool									_globalConf;
};

#endif //CONFIG_H__

