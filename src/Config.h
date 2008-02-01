#ifndef CONFIG_H__
# define CONFIG_H__

#include <vector>
#include <map>
#include <string>

class Config
{
	public:
		Config();
		/// Copy constructor, to allow configuration inheritance.
		Config(const Config&);
		~Config();
		/// Will return the param value as a string.
		std::string								getParam(std::string) const;
		/// Used to set a param, while parsing XML file.
		void									setParam(std::string, std::string);
	private:
		/// The param list, stocked as name => value
		std::map<std::string, std::string>		_params;
};

class RootConfig : Config
{
	public:
		RootConfig();
		~RootConfig();
		/// Will get all global vhosts
		std::vector<Config*>				getVhosts() const;
		/// Will get all VHosts by address, for every ports
		std::vector<Config*>				getVhosts(std::string) const;
		/// Will get all VHosts for adress:port specified.
		std::vector<Config*>				getVhosts(std::string, int) const;
	private:
		std::map<std::string, std::map<int, std::vector<Config*> > >		_address;
		std::vector<Config*>												_globalVhosts;
};

#endif //CONFIG_H__

