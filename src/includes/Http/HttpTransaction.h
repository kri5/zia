#ifndef HTTP_TRANSACTION_H__
# define HTTP_TRANSACTION_H__

#include <map>
#include <string>
#include <sstream>
#include "API/IHttpTransaction.h"

typedef std::map<std::string, std::string>      SSMap;

class   HttpTransaction : public IHttpTransaction
{
    public:
        const SSMap&            getHeaderOptions() const;
        bool                    headerOptionIsSet(std::string) const;
        std::string				getHeaderOption(std::string) const;

        const std::string&      getProtocol() const;
        void                    setProtocol(std::string&);
        template <typename T>
        void                    setHeaderOption(std::string opt, T value)
        {
            std::ostringstream  s;

            s << value;
            this->_options[opt] = s.str();
        }
        void                    setHeaderOption(std::string, std::string&);
    protected:
        std::string             _protocol;
        SSMap                   _options;

};

#endif //HTTP_TRANSACTION_H__
