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

HttpParser::HttpParser() :  _isFirstArgument(true),
                            _isFirstLine(true),
                            _isValid(true),
                            _isDone(false)
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
    if (this->_isFirstLine)
    {
        if (this->parseGetCommand()
            || this->parsePostCommand())
            this->_isFirstLine = false;
        else
            this->_isValid = false;
    }
    else if (this->_isValid)
	{
        while (this->parseOptions())
			;
	}
    this->flush();
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
    std::string      token;

	if (this->peekIfEqual("GET")
        && this->parseUri()
        && this->parseProtocol()
        && this->isEOL())
    {
        this->_request->setCommand(HttpRequest::Get);
        return true;
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

    if (this->peekIfEqual("POST"))
    {
        if (this->parseUri())
        {
            if (this->parseProtocol())
            {
                if (this->isEOL())
                {
                    this->_request->setCommand(HttpRequest::Post);
                    return true;
                }
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

    this->setCommentList("#");
    this->setComment(true);
    if (this->readAbsoluteUri(token)
        || this->readRelativeUri(token))
    {
		this->setIgnore(false);
        this->_request->setUri(token);
        this->parseUriArgument();
        this->setComment(false);
		this->setIgnore(true);
		return true;
    }
    this->setComment(false);
	this->setIgnore(true);
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

	// URI_Argument ::= [ '?' [param]*]?
    if (this->peekIfEqual('?'))
    {
		if (this->readUriParam(key, value))
		{
			this->_request->appendUriArgument(key, value);
			while (this->peekIfEqual('&'))
			{
				this->readUriParam(key, value);
				this->_request->appendUriArgument(key, value);
			}
		}
    }
    return true;
}

bool        HttpParser::readUriParam(std::string& key, std::string& value)
{
	// URI_Param ::= [ #paramIdentifier '=' #paramIdentifier ]
	// paramIdentifier ::= [#idenfifier | #integer | #double | "!@$%^&*().<>/\[]{}\"'" | #urlEncodedChar ]
	// which is anything but "&" and "="
	// if we made it to an ignore char, we return false (end of the URI params)

	if (this->isIgnore(this->readChar()))
		return false;
	if (this->readAnythingBut("#&=", key) && this->peekIfEqual('='))
	{
		this->readAnythingBut("#&", value);
	}
	else //Going to the hypothetic next 
	{
		this->peekIfEqual('=');
		this->readAnythingBut("#&");
	}
	return true;
}

/**
 *  Parses Header options :
 *  try to parse every http options
 *  "yet" implemented in the zia
 * */

bool        HttpParser::parseOptions()
{
	//if (this->parseOption("Host", HttpRequest::From)
	//	|| this->parseOption("Content-Length", HttpRequest::ContentLength)
	//	|| this->parseOption("From", HttpRequest::From)
	//	|| this->parseOption("User-Agent", HttpRequest::UserAgent)
	//	|| this->parseOption("Content-Type", HttpRequest::ContentType)
	//	|| this->parseOption("Date", HttpRequest::Date))
    //    return true;
	//std::cout << "Option return false" << std::endl;
    //return false;

    if (this->parseOptionHost()
        || this->parseOptionFrom()
        || this->parseOptionContentLength()
        || this->parseOptionDate()
        || this->parseOptionContentType()
        || this->parseOptionUserAgent())
    {
        if (this->isEOL())
        {
            if (this->isEOL())
                this->_isDone = true;
            return true;
        }
        else
            std::cout << " === ERROR === " << std::endl;
    }
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
    std::string host;

    if (this->peekIfEqual("Host"))
    {
        if (this->peekIfEqual(":"))
        {
            if (this->readHost(host))
            {
                this->_request->appendOption
                    (HttpRequest::Host,host);
                return true;
            }
        }
    }
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
    std::string from;

    if (this->peekIfEqual("From"))
    {
        if (this->peekIfEqual(":"))
        {
            this->readUntilEndOfLine(from);
            this->_request->appendOption
                (HttpRequest::From, from);
            return true;
        }
    }
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
    std::string length;

    if (this->peekIfEqual("Content-Length"))
    {
        if (this->peekIfEqual(":"))
        {
            if (this->readInteger(length))
            {
                this->_request->appendOption
                    (HttpRequest::ContentLength, length);
                return true;
            }
        }
    }
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
    std::string date;

    if (this->peekIfEqual("Date"))
    {
        if (this->peekIfEqual(":"))
        {
            if (this->readDate(date))
            {
                this->_request->appendOption
                    (HttpRequest::Date, date);
                return true;
            }
        }
    }
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
    std::string userAgent;

    if (this->peekIfEqual("User-Agent"))
    {
        if (this->peekIfEqual(":"))
        {
            this->readUntilEndOfLine(userAgent);
            this->_request->appendOption
                (HttpRequest::UserAgent, userAgent);
            return true;
        }
    }
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
    std::string contentType;

    if (this->peekIfEqual("Content-Type"))
    {
        if (this->peekIfEqual(":"))
        {
            this->readUntilEndOfLine(contentType);
            this->_request->appendOption
                (HttpRequest::ContentType, contentType);
            return true;
        }
    }
    return false;
}


bool        HttpParser::parseProtocol()
{
    std::string     token;

    if (this->peekIfEqual("HTTP/1.0", token)
		|| this->peekIfEqual("HTTP/1.1"))
    {
        this->_request->setProtocol(token);
        return true;
    }
    return false;
}

/**
 *  Reads a host
 *  the host must be formatted as follows
 *  [identifier][.][identifier][.identifier]*
 * */

bool        HttpParser::readHost(std::string& host)
{
    std::string tmp;

    if (this->appendIdentifier(tmp))
    {
        if (this->peekIfEqual(".", tmp))
        {
            if (this->appendIdentifier(tmp))
            {
                while (this->peekIfEqual(".", tmp)
                        && this->appendIdentifier(tmp));
                if (this->peekIfEqual(":", tmp))
                {
                    if (!this->appendInteger(tmp))
                        return false;
                }
                host = tmp;
                return true;
            }
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
        if (this->peekChar() == '@')
        {
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

bool        HttpParser::readAbsoluteUri(std::string& uri)
{
    std::string res;

	this->saveContextPub();
    if (this->peekIfEqual("http://", res))
    {
		this->setIgnore(false);
        if (this->appendIdentifier(res))
        {
             while (this->appendIdentifier(res)
                    || this->peekIfEqual('.', res)
                    || this->peekIfEqual('/', res));
             uri = res;
             this->setIgnore(true);
             return true;
        }
    }
    this->setIgnore(true);
    this->restoreContextPub();
    return false; 
}

bool        HttpParser::readRelativeUri(std::string& uri)
{
    std::string res;

    if (this->peekIfEqual("/", res))
    {
        while (this->appendIdentifier(res)
               || this->peekIfEqual(".", res)
               || this->peekIfEqual("/", res));
        uri = res;
        return true;
    }
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
        && this->isDayOfTheWeek(tmp))
    {
        res += tmp;
        if (this->peekChar() == ',')
        {
            res += ", ";
            if (this->readInteger(tmp)
                && this->isDayOfTheMonth(atoi(tmp.c_str())))
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
    if (this->readInteger(tmp)
        && this->isHour(atoi(tmp.c_str())))
    {
        res += tmp;
        if (this->readChar() == ':')
        {
            res += this->peekChar();
            if (this->readInteger(tmp)
                && this->isMinute(atoi(tmp.c_str())))
            {
                res += tmp;
                if (this->readChar() == ':')
                {
                    res += this->peekChar();
                    if (this->readInteger(tmp)
                        && this->isSecond(atoi(tmp.c_str())))
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
 *  Checks if the hour is valid
 *  (between 0 and 24)
 * */

bool    HttpParser::isHour(const int h) const
{
    if (h >= 0 && h < 24)
        return true;
    return false;
}


/**
 *  Checks if the minut is valid
 *  (between 0 and 60)
 * */

bool    HttpParser::isMinute(const int m) const
{
    if (m >= 0 && m < 60)
        return true;
    return false;
}


/**
 *  Checks if the second is valid
 *  (between 0 and 60)
 * */

bool    HttpParser::isSecond(const int s) const
{
    if (s >= 0 && s < 60)
        return true;
    return false;
}


/**
 *  Checks if the day is valid
 *  (between 1 and 31)
 * */

bool    HttpParser::isDayOfTheMonth(const int d) const
{
    if (d > 0 && d <= 31)
        return true;
    return false;
}


/**
 *  Checks whether the passed string
 *  is a day of the week or not
 * */

bool    HttpParser::isDayOfTheWeek(const std::string& d) const
{
    if (d == "Mon" 
        || d == "Tue"
        || d == "Wed" 
        || d == "Thu"
        || d == "Fri"
        || d == "Sat"
        || d == "Sun")
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

HttpRequest*    HttpParser::getRequest() const
{
    return this->_request;
}

bool            HttpParser::done() const
{
    return this->_isDone;
}
