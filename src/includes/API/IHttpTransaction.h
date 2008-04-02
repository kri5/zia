#ifndef __IHTTPTRANSACTION_H__
# define __IHTTPTRANSACTION_H__

#include <string>
#include <map>

class IHttpTransaction
{
    public:
        /**
         *  The interface virtual destructor
         */
        virtual ~IHttpTransaction() {};

        /**
         *  Sets a header option with a key / value pair passed in parameters
         *  @param key the key of the header option to set
         *  @param value the value of the option
         */
        virtual void                                        setHeaderOption(const std::string& key, const std::string& value) = 0;

        /**
         *  Returns the value matching the key passed in parameter.
         *  Returns NULL if the key was not set yet
         *  @return the value of the option
         */
        virtual std::string                                 getHeaderOption(const std::string& key) const = 0;

        /**
         *  Returns a string/string map containing all the header options
         *  @return the options map
         */
        virtual const std::map<std::string, std::string>&   getHeaderOptions() const = 0;

        /**
         *  Checks wheter the header options matches the key passed in parameter
         *  @param key the key to check
         *  @return true if the entry exists, else otherwise
         */
        virtual bool                                        headerOptionIsSet(const std::string& key) const = 0;

        /**
         *  Set the header protocol (typically "HTTP/1.1" or "HTTP/1.0")
         *  @param protocol the header protocol
         */
        virtual void                                        setProtocol(const std::string& protocol) = 0;

        /**
         *  Returns the current header protocol
         *  @return the header protocol
         */
        virtual const std::string&                          getProtocol() const = 0;
};

# endif /* !__IHTTPTRANSACTION_H__ */
