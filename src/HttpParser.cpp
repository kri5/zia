#include "HttpParser.h"
#include <string>

HttpParser::HttpParser()
{
    _request = new HttpRequest();
}


void        HttpParser::parse()
{
    if (this->parseCommand())
        while (this->parseOption());
    this->_request->print();
}

bool        HttpParser::parseCommand()
{
    std::string     token;

    if (this->readIdentifier(token))
    {
        if (token == "GET" || token == "POST")
        {
            this->_request->setCommand(token);
            if (this->parseUri())
            {
                if (this->parseProtocol())
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool        HttpParser::parseUri()
{
    std::string     token;

    if (this->readIdentifier(token))
    {
        this->_request->setUri(token);
        return true;
    }
    return false;
}

bool        HttpParser::parseOption()
{
    std::string     token;
    std::string     token2;
    if (this->readIdentifier(token))
    {
        if (this->readChar() == ':')
        {
            this->peekChar();
            if (this->readIdentifier(token2))
            {
                this->_request->appendOption(token, token2);
                return true;
            }
        }
    }
    
    return false;
}

bool        HttpParser::parseProtocol()
{
    std::string     token;

    if (this->readIdentifier(token))
    {
        if (token == "HTTP/1.0" || token == "HTTP/1.1")
        {
            this->_request->setProtocol(token);
            return true;
        }
    }
    return false;
}
