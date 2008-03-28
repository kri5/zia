#include "Http/HttpTransaction.h"

void        HttpTransaction::setProtocol(const std::string& protocol)
{
    this->_protocol = protocol;
}

bool    HttpTransaction::headerOptionIsSet(const std::string& opt) const
{
    return (this->_options.find(opt) != this->_options.end());
}

const std::string&      HttpTransaction::getProtocol() const
{
    return this->_protocol;
}

const SSMap&    HttpTransaction::getHeaderOptions() const
{
    return this->_options;
}

std::string       HttpTransaction::getHeaderOption(const std::string& option) const
{
    std::map<std::string, std::string>::const_iterator    it = this->_options.find(option);
    if (it != this->_options.end())
        return it->second;
    return "";
}

void            HttpTransaction::setHeaderOption(const std::string& key,
                                          const std::string& value)
{
    this->_options[key] = value;
}

