#include "Http/HttpTransaction.h"

void        HttpTransaction::setProtocol(std::string& protocol)
{
    this->_protocol = protocol;
}

const std::string&      HttpTransaction::getProtocol() const
{
    return this->_protocol;
}

const SSMap&    HttpTransaction::getOptions() const
{
    return this->_options;
}

std::string       HttpTransaction::getOption(std::string option) const
{
    std::map<std::string, std::string>::const_iterator    it = this->_options.find(option);
    if (it != this->_options.end())
        return it->second;
    return "";
}

void            HttpTransaction::appendOption(std::string key,
                                          std::string& value)
{
    this->_options[key] = value;
}

