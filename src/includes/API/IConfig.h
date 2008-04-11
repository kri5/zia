#ifndef ICONFIG_H__
# define ICONFIG_H__

#include <string>

namespace zAPI
{
    /// Configuration interface.
    class   IConfig
    {
        public:

            /**
             *  The interface virtual destructor
             */
            virtual ~IConfig(){}

            /**
             *  Returns the value of the configuration parameter key passed in paramater as a string.
             *  If the key is not found, NULL will be returned instead
             *  @param key a configuration parameter key
             *  @return a configuration parameter value
             */
            virtual const std::string*                  getParam(const std::string& key) const = 0;

            /**
             *  Returns the value of the configuration parameter key passed in paramater as a char*.
             *  If the key is not found, NULL will be returned instead
             *  @param key a configuration parameter key
             *  @return a configuration parameter value
             */
            virtual const char*                         getParamChar(const std::string& key) const = 0;

            /**
             *  Sets a configuration parameter with a key / value pair passed in parameter
             *  @param key the configuration parameter key
             *  @param value the configuration parameter value
             */
            virtual void                                setParam(const std::string& key, const std::string& value) = 0;

            /**
             *  Checks whether a certain parameter key is set or not
             *  @param key a configuration parameter key
             *  @return whether the key is set or not
             */
            virtual bool                                isSet(const std::string& key) const = 0;
    };
}

#endif //ICONFIG_H__

