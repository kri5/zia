#include "Http/HttpParser.h"
#include "Http/HttpRequest.h"
#include "Logger.hpp"

#include <string>


/**
 *  CLASS : HttpParser
 *
 *  this class is a llParser of the
 *  HTTP protocol. It construct an
 *  HttpRequest with all parameters inside
 * */

HttpParser::HttpParser(HttpRequest* req, Buffer* buff) : Parser(buff),
                            _request(req),
                            _isFirstArgument(true),
                            _isFirstLine(true),
                            _isValid(true),
                            _isDone(false)
{
}


HttpParser::~HttpParser()
{
}

void    HttpParser::init()
{
    this->_isFirstArgument = true;
    this->_isFirstLine = true;
    this->_isValid = true;
    this->_isDone = false;
    Parser::init();
}

/**
 *  Checks if the parsed request is valid or not
 *  @return true if the request is valid, false otherwise
 */
bool        HttpParser::isValid() const
{
    return this->_isValid;
}

bool        HttpParser::isFirstLine() const
{
    return this->_isFirstLine;
}

/**
 *  tries to parse any implemented
 *  Http command and then if it succeeds
 *  parses options as long as it can
 * */

void        HttpParser::parse()
{
//    std::cout << "||||| parser flag is fed " << this->isFed() << std::endl
//        << "isDone " << this->_isDone << std::endl
//        << "isValid " << this->_isValid << std::endl
//        << "isFirstLine " << this->_isFirstLine << std::endl
//        << "isFirstArg " << this->_isFirstArgument << std::endl
//        << "isEnd " << this->isEnd() << std::endl;
    if (!this->isFed())
        return ;
    if (this->_isFirstLine && this->hasEOL())
    {
        if (this->parseGetCommand()
            || this->parsePostCommand()
            || this->parseHeadCommand()
            || this->parseGenericCommand())
        {
            this->_isValid = true;
            if (this->isEOL())
            {
                this->_isDone = true;
            }
            this->_isFirstLine = false;
        }
        else
            this->_isValid = false;
    }
    if (this->_isValid && this->_isFirstLine == false)
	{
        if (this->isEOL())
            this->_isDone = true;
        while (!this->isEnd() && this->hasEOL() && this->parseOptions())
        {
            //FIXME: remove me after debug, causea i'm kind of slow.
            //this->flush();
        }
	}
    if (this->_isDone)
    {
        //std::cout << "\n\n************PRE DUMP************\n\n" << std::endl;
        //this->_buffer->dump();
        //std::cout << "\n\n************POST DUMP************\n\n" << std::endl;
        this->flush();
        //std::cout << "\n\n************PRE FLUSH************\n\n" << std::endl;
        //this->_buffer->dump();
        //std::cout << "\n\n************POST FLUSH************\n\n" << std::endl;
    }
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
    if (this->peekIfEqual("GET"))
    {
        if (this->parseUri())
        {
            if (this->parseProtocol())
            {
                if (this->isEOL())
                {
                    this->_request->setCommand("GET");
                    return true;
                }
            }
        }
    }
    return false;
}

bool        HttpParser::parseGenericCommand()
{
    std::string command;

    if (this->readIdentifier(command))
    {
        if (this->parseUri())
        {
            if (this->parseProtocol())
            {
                if (this->isEOL())
                {
                    this->_request->setCommand(command);
                    return true;
                }
            }
        }
    }
    return false;
}


/**
 *  Parses HEAD Command
 *  
 *  synopsis:
 *      HEAD [path] Protocol\r\n
 *
 *  examples:
 *      HEAD /pub/ HTTP/1.1
 *      HEAD http://slashdot.org/ HTTP/1.1
 *      HEAD HTTP/1.1
 * */

bool        HttpParser::parseHeadCommand()
{
    if (this->peekIfEqual("HEAD"))
    {
        if (this->parseUri())
        {
            if (this->parseProtocol())
            {
                if (this->isEOL())
                {
                    this->_request->setCommand("HEAD");
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

    if (this->peekIfEqual("POST"))
    {
        if (this->parseUri())
        {
            if (this->parseProtocol())
            {
                if (this->isEOL())
                {
                    this->_request->setCommand("POST");
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
    bool            relative = true;

    this->setCommentList("#");
    this->setComment(true);
    if (this->readAbsoluteUri(token, relative)
        || this->readRelativeUri(token, relative)
        || this->peekIfEqual("*", token))
    {
		this->setIgnore(false);
        this->_request->setUri(token, relative);
        this->parseUriArgument();
        this->setComment(false);
		this->setIgnore(true);
		return true;
    }
    this->setComment(false);
	this->setIgnore(true);
    return false;
}

bool        HttpParser::parseUriArgument()
{
    std::string     uriQuery;

	// URI_Argument ::= [ '?' [param]*]?
//    if (this->peekIfEqual('?'))
//    {
//		if (this->readParam(key, value))
//		{
//			//this->_request->setUriArgument(key, value);
//			while (this->peekIfEqual('&'))
//			{
//				this->readParam(key, value);
//				//this->_request->setUriArgument(key, value);
//			}
//		}
//    }
    this->setIgnore(false);
    if (this->peekIfEqual('?') == false)
    {
        this->_request->setUriQuery("");
        return false;
    }
    this->readAnythingBut(" \t", uriQuery);
    this->setIgnore(true);
    this->_request->setUriQuery(uriQuery);
    return true;
}
	// URI_Param ::= [ #paramIdentifier '=' #paramIdentifier ]
	// paramIdentifier ::= [#idenfifier | #integer | #double 
    // | "!@$%^&*().<>/\[]{}\"'" | #urlEncodedChar ]
	// which is anything but "&" and "="
	// if we made it to an ignore char, we return false 
    // (end of the URI params)

bool        HttpParser::readParam(std::string& key, 
                                     std::string& value)
{
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
    if (this->parseOptionHost()
        || this->parseOptionConnection()
        || this->parseOptionFrom()
        || this->parseOptionContentLength()
        || this->parseOptionDate()
        || this->parseOptionContentType()
        || this->parseOptionUserAgent()
        || this->parseOptionGeneric())
    {
        if (this->isEOL())
        {
            if (this->isEOL())
            {
                this->_isDone = true;
                return false;
            }
            return true;
        }
    }
    this->_isValid = false;
    //this->readUntilEndOfLine();
    return false;
}

/**
 *  This method just ignore a header option
 *  that we dont recognize. If all parseOptionXxx
 *  methods return false, this method is call to
 *  skip the unknowed option
 * */

bool        HttpParser::parseOptionGeneric()
{
    std::string value;
    std::string key;

    this->saveContextPub();
    if (this->readAnythingBut(":", key))
    {
        if (this->peekIfEqual(":"))
        {
            this->readUntilEndOfLine(value);
            this->_request->setHeaderOption(key, value);
            return true;
        }
    }
    this->restoreContextPub();
    return false;
}

/**
 * Parses Accept option
 *
 * synopsis:
 *  "Accept" ":" #( media-range [ accept-params ] )
 *  media-range    = ( "*\/\*"
 *                   | ( type "/" "*" )
 *                   | ( type "/" subtype )
 *                   ) *( ";" parameter )
 *  accept-params  = ";" "q" "=" qvalue *( accept-extension )
 *  accept-extension = ";" token [ "=" ( token | quoted-string ) ]
 *
 *  examples:
 *      Accept: audio\/\*; q=0.2, audio/basic
 *      Accept: text\/\*, text/html, text/html;level=1, *\\/\*
 * */

bool        HttpParser::parseOptionAccept()
{
    std::string     token;

    if (this->peekIfEqual("Accept"))
    {
        if (this->peekIfEqual(":"))
        {
            if (this->readAcceptType(token))
            {
                while (this->peekIfEqual(",", token)
                        && this->readAcceptType(token))
                    ;
                this->_request->setHeaderOption
                    ("Accept", token);
                return true;
            }
        }
    }
    return false;
}

bool        HttpParser::readAcceptType(std::string& token)
{
    std::string tmp = "";
    std::string tmpTok;

    if (this->readIdentifier(tmpTok)
        || this->peekIfEqual("*", tmpTok))
    {
        tmp += tmpTok;
        tmpTok = "";
        if (this->peekIfEqual("/", tmp))
        {
            if (this->readIdentifier(tmpTok)
                || this->peekIfEqual("*", tmpTok))
            {
                tmp += tmpTok;
                token += tmp;
                while (this->readAcceptParam(token));
                return true;
            }
        }
    }
    return false;
}

bool        HttpParser::readAcceptParam(std::string& token)
{
    std::string     tmp;
    std::string     tmpTok;

    if (this->peekIfEqual(";", tmp))
    {
        if (this->readIdentifier(tmpTok))
        {
            tmp += tmpTok;
            if (this->peekIfEqual("=", tmp))
            {
                if (this->readIdentifier(tmpTok)
                    || this->readDecimal(tmpTok))
                {
                    tmp += tmpTok;
                    token += tmp;
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 *  Parses Host connection
 *
 *  synopsis:
 *      "Connection" ":" 1#(connection-token)
 *  example:
 *      Connection: close
 * */

bool        HttpParser::parseOptionConnection()
{
    std::string token;

    if (this->peekIfEqual("Connection"))
    {
        if (this->peekIfEqual(":"))
        {
            if (this->readIdentifier(token))
            {
                this->_request->setHeaderOption
                    ("Connection", token);
                return true;
            }
        }
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
                this->_request->setHeaderOption
                    ("Host", host);
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
            this->_request->setHeaderOption
                ("From", from);
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
                this->_request->setHeaderOption
                    ("Content-Length", length);
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
                this->_request->setHeaderOption
                    ("Date", date);
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
            this->_request->setHeaderOption
                ("UserAgent", userAgent);
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
            this->_request->setHeaderOption
                ("Content-Type", contentType);
            return true;
        }
    }
    return false;
}


bool        HttpParser::parseProtocol()
{
    std::string     token;

    if (this->peekIfEqual("HTTP/1.0", token)
		|| this->peekIfEqual("HTTP/1.1", token))
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
    else
        if (this->appendInteger(tmp)
            && this->peekIfEqual(".", tmp))
        {
            if (this->appendInteger(tmp)
                && this->peekIfEqual(".", tmp))
            {
                if (this->appendInteger(tmp)
                    && this->peekIfEqual(".", tmp))
                {
                    if (this->appendInteger(tmp))
                    {
                        if (this->peekIfEqual(":", tmp)
                            && !this->appendInteger(tmp))
                            return false;
                        host = tmp;
                        return true;
                    }
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

/**
 *  Reads and Absolute Uri
 *  Basic synosis must look like this :
 *  http://identifier[identifier|'.'|'/']
 *
 * This method also append e Host option 
 * to the request object
 * */

bool        HttpParser::readAbsoluteUri(std::string& uri,
                                        bool& relative)
{
    std::string res;
    std::string host;

	this->saveContextPub();
    if (this->peekIfEqual("http://", res))
    {
		this->setIgnore(false);
        if (this->appendIdentifier(res))
        {
             while (this->appendIdentifier(res)
                    || this->peekIfEqual('.', res)
                    || this->peekIfEqual('/', res))
             {
                if (this->getLastReadChar() == '/')
                {
                    host = res.substr(7, res.length() - 8);
                    this->_request->setHeaderOption("Host", host);
                }
             }
             uri = res;
             relative = false;
             this->setIgnore(true);
             return true;
        }
    }
    this->setIgnore(true);
    this->restoreContextPub();
    return false; 
}

/**
 *  Reads a relative Uri
 *  Basic synosis must look like this :
 *  /[identifier|num|'.'|'/']
 * */

bool        HttpParser::readRelativeUri(std::string& uri,
                                        bool& relative)
{
    std::string res;

    this->setIgnore(false);
    if (this->peekIfEqual("/", res))
    {
        while (this->appendIdentifier(res)
               || this->peekIfEqual(".", res)
               || this->peekIfEqual("/", res)
               || this->peekIfNum(res))
        uri = res;
        relative = true;
        this->setIgnore(true);
        return true;
    }
    this->setIgnore(true);
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
