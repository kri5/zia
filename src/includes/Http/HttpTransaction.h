#ifndef HTTP_TRANSACTION_H__
# define HTTP_TRANSACTION_H__

#include <map>
#include <string>
#include <sstream>

typedef std::map<std::string, std::string>      SSMap;

class   HttpTransaction
{
    public:
        const SSMap&            getOptions() const;
        bool                    optionIsSet(std::string) const;
        std::string				getOption(std::string) const;

        const std::string&      getProtocol() const;
        void                    setProtocol(std::string&);
        template <typename T>
        void        appendOption(std::string opt, T value)
        {
            std::ostringstream  s;

            s << value;
            this->_options[opt] = s.str();
        }
        void                    appendOption(std::string, std::string&);
    protected:
        std::string             _protocol;
        SSMap                   _options;

};

#endif //HTTP_TRANSACTION_H__
