#include "HttpParser.h"
#include "HttpRequest.h"
#include <string>


/**
 *  CLASS : HttpParser
 *
 *  this class is a llParser of the
 *  HTTP protocol. It construct an
 *  HttpRequest with all parameters inside
 * */

HttpParser::HttpParser()
{
    _request = new HttpRequest();
}



/**
 *  tries to parse any implemented
 *  Http command and then if it succeeds
 *  parses options as long as it can
 * */

void        HttpParser::parse()
{
    if (this->parseGetCommand()
        || this->parsePostCommand())
        while (this->parseOption());
    this->_request->print();
}


/**
 *  Parses GET Command
 *  
 *  synopsis:
 *      GET [path] Protocol\r\n
 *
 *  examples:
 *      GET /pub/ HTTP/1.1
 *      GET http://slashdot.org/ HTTP/1.1
 *      GET HTTP/1.1
 * */

bool        HttpParser::parseGetCommand()
{
    std::string     token;

    if (this->readIdentifier(token))
    {
        if (token == "GET")
        {
            this->_request->setCommand(HttpRequest::Get);
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


/**
 *  Parses POST Command
 *
 *  synopsis:
 *      POST [path] Protocol\r\n
 *
 *  examples:
 *      POST /index.php?module=Test HTTP/1.1\r\n
 *      
 * */

bool        HttpParser::parsePostCommand()
{
    std::string     token;

    if (this->readIdentifier(token))
    {
        if (token == "POST")
        {
            this->_request->setCommand(HttpRequest::Post);
            if (this->parseUri())
            {
                if (this->parseProtocol())
                    return true;
            }
        }
    }
    return false;
}


/**
 *  Parses URI
 *
 *  /path/to/file/          [relative URI]
 *      OR
 *  http://linuxfr.org/pub/ [absolute URI]
 * */

bool        HttpParser::parseUri()
{
    std::string     token;

    if (this->readIdentifier(token))
    {
        this->_request->setUri(token);
        while (this->parseUriArgument());
        return true;
    }
    return false;
}


/**
 *  Parses a key => value pair
 *  in the uri. For now this version
 *  is too permissive, you're allowed
 *  to do such ugly things : 
 *  /index.php?test=42?foo=bar
 *  needs to be fixed
 * */

bool        HttpParser::parseUriArgument()
{
    std::string     key;
    std::string     value;

    this->saveContextPub();
    if (this->readChar() == '?'
        || this->readChar() == '&')
    {
        this->peekChar();
        if (this->readIdentifier(key))
        {
            if (this->readChar() == '=')
            {
                this->peekChar();
                if (this->readIdentifier(value))
                {
                    this->_request->appendUriArgument(key, value);
                    return true;
                }
            }
        }
    }
    this->restoreContextPub();
    return false; 
}


/**
 *  Parses Header options :
 *  try to parse every http options
 *  "yet" implemented in the zia
 * */

bool        HttpParser::parseOption()
{
    if (this->parseOptionHost()
        || this->parseOptionContentLength()
        || this->parseOptionFrom()
        || this->parseOptionUserAgent()
        || this->parseOptionContentType()
        || this->parseOptionDate())
        return true;
    return false;
}


/**
 *  Parses Host option
 *
 *  synopsis:
 *      "Host" ":" host [ ":" port ]
 *
 *  example:
 *      Host: www.w3c.org\r\n
 * */

bool        HttpParser::parseOptionHost()
{
    std::string     token;
    std::string     token2;

    this->saveContextPub();
    if (this->readIdentifier(token))
    {
        if (token == "Host")
        {
            if (this->readChar() == ':')
            {
                this->peekChar();
                if (this->readIdentifier(token2))
                {
                    this->_request->appendOption
                        (HttpRequest::Host, token2);
                    return true;
                }
            }
        }
    }
    this->restoreContextPub();
    return false;
}


/**
 *  Parses From option
 *
 *  synopsis:
 *      "From" ":" mailbox    
 *
 *  example:
 *      From: webmaster@w3.org
 * */

bool        HttpParser::parseOptionFrom()
{
    std::string     token;
    std::string     token2;

    this->saveContextPub();
    if (this->readIdentifier(token))
    {
        if (token == "From")
        {
            if (this->readChar() == ':')
            {
                this->peekChar();
                if (this->readEmailAddress(token2))
                {
                    this->_request->appendOption
                        (HttpRequest::From, token2);
                    return true;
                }
            }
        }
    }
    this->restoreContextPub();
    return false;
}


/**
 *  Parses Content-Length option
 *
 *  synopsis:
 *      "Content-Length" ":" 1*DIGIT
 *
 *  example:
 *      Content-Length: 3495
 * */

bool        HttpParser::parseOptionContentLength()
{
    std::string     token;
    std::string     token2;

    this->saveContextPub();
    if (this->readIdentifier(token))
    {
        if (token == "Content-Length")
        {
            if (this->readChar() == ':')
            {
                this->peekChar();
                if (this->readInteger(token2))
                {
                    this->_request->appendOption
                        (HttpRequest::ContentLength, token2);
                    return true;
                }
            }
        }
    }
    this->restoreContextPub();
    return false;
}


/**
 *  Parses User-Agent option
 *
 *  synopsis:
 *      "User-Agent" ":" 1*( product | comment )
 *
 *  example:
 *      User-Agent: CERN-LineMode/2.15 libwww/2.17b3
 * */

bool        HttpParser::parseOptionUserAgent()
{
    std::string     token;
    std::string     token2;

    this->saveContextPub();
    if (this->readIdentifier(token))
    {
        if (token == "User-Agent")
        {
            if (this->readChar() == ':')
            {
                this->peekChar();
                if (this->readIdentifier(token2))
                {
                    this->_request->appendOption
                        (HttpRequest::UserAgent, token2);
                    return true;
                }
            }
        }
    }
    this->restoreContextPub();
    return false;
}


/**
 *  Parses Content-Type option
 *
 *  synopsis:
 *      "Content-Type" ":" media-type
 *
 *  example:
 *      Content-Type: text/html; charset=ISO-8859-4
 * */

bool        HttpParser::parseOptionContentType()
{
    std::string     token;
    std::string     token2;

    this->saveContextPub();
    if (this->readIdentifier(token))
    {
        if (token == "Content-Type")
        {
            if (this->readChar() == ':')
            {
                this->peekChar();
                if (this->readIdentifier(token2))
                {
                    this->_request->appendOption
                        (HttpRequest::ContentType, token2);
                    return true;
                }
            }
        }
    }
    this->restoreContextPub();
    return false;
}


/**
 *  Parses Date option
 *
 *  synopsis:
 *      "Date" ":" HTTP-date
 *
 *  example:
 *      Date: Tue, 15 Nov 1994 08:12:31 GMT
 * */

bool        HttpParser::parseOptionDate()
{
    std::string     token;
    std::string     token2;

    this->saveContextPub();
    if (this->readIdentifier(token))
    {
        if (token ==  "Date")
        {
            if (this->readChar() == ':')
            {
                this->peekChar();
                if (this->readDate(token2))
                {
                    this->_request->appendOption
                        (HttpRequest::Date, token2);
                    return true;
                }
            }
        }
    }
    this->restoreContextPub();
    return false;
}


/**
 *  Parses protocol
 *  checks whether protocol is
 *  HTTP/1.1 or HTTP/1.0, sets the
 *  found protocol or return false
 * */

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


/**
 *  Reads an email address.
 *  The address must be 
 *  formated like this :
 *  [identifier]@[host].[domain]
 * */

bool        HttpParser::readEmailAddress(std::string& email)
{
    std::string     token;
    std::string     token2;

    this->saveContextPub();
    if (this->readIdentifier(token))
    {
        if (this->readChar() == '@')
        {
            this->peekChar();
            if (this->readIdentifier(token2))
            {
                email = token + '@' + token2;
                return true;
            }
        }
    }
    this->restoreContextPub();
    return false;
}


/**
 *  Reads an HTTP date
 *
 *  synopsis:
 *      "Date" ":" HTTP-date
 *
 *  example:
 *      Date: Tue, 15 Nov 1994 08:12:31 GMT
 * */

bool        HttpParser::readDate(std::string& date)
{
    std::string     res;
    std::string     tmp;

    this->saveContextPub();
    if (this->readIdentifier(tmp)
        && this->isWeekOfTheDay(tmp))
    {
        res += tmp;
        if (this->readChar() == ',')
        {
            res += this->peekChar();
            res += ' ';
            if (this->readInteger(tmp))
            {
                res += tmp + ' ';
                if (this->readIdentifier(tmp)
                    && this->isMonthOfTheYear(tmp))
                {
                    res += tmp + ' ';
                    if (this->readInteger(tmp))
                    {
                        res += tmp + ' ';
                        if (this->readTime(tmp))
                        {
                            res += tmp;
                            if (this->readIdentifier(tmp)
                                && tmp == "GMT")
                            {
                                res += " GMT";
                                date = res;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    this->restoreContextPub();
    return false;
}


/**
 *  Reads the time part of the previous date.
 *
 *      example:
 *          19:42:07
 * */

bool    HttpParser::readTime(std::string& t)
{
    std::string tmp;
    std::string res;

    this->saveContextPub();
    if (this->readInteger(tmp))
    {
        res += tmp;
        if (this->readChar() == ':')
        {
            res += this->peekChar();
            if (this->readInteger(tmp))
            {
                res += tmp;
                if (this->readChar() == ':')
                {
                    res += this->peekChar();
                    if (this->readInteger(tmp))
                    {
                        res += tmp;
                        t = res;
                        return true;
                    }
                }
            }
        }
    }
    this->restoreContextPub();
    return false;
}


/**
 *  Checks whether the passed string
 *  is a day of the week or not
 * */

bool    HttpParser::isWeekOfTheDay(const std::string& d) const
{
    if (d == "Mon" 
        || d == "Tue"
        || d == "Wed" 
        || d == "Thr"
        || d == "Fri"
        || d == "Sat"
        || d == "sun")
        return true;
    return false;
}


/**
 *  Checks whether the passed string
 *  is a month of the year
 * */

bool    HttpParser::isMonthOfTheYear(const std::string& m) const
{
    if (m == "Jan"
        || m == "Feb"
        || m == "Mar"
        || m == "Apr"
        || m == "May"
        || m == "Jun"
        || m == "Jul"
        || m == "Aug"
        || m == "Sep"
        || m == "Oct"
        || m == "Nov"
        || m == "Dec")
        return true;
    return false;
}
