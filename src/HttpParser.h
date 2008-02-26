#ifndef __HTTPPARSER_H__
# define __HTTPPARSER_H__

#include "Parser.h"
#include "HttpRequest.h"

class HttpParser : public Parser
{
    public:
        HttpParser();
        ~HttpParser() {};
        void            parse();
        bool            done() const;
        HttpRequest*    getRequest() const;

    private:
        bool            parseGetCommand();
        bool            parsePostCommand();
        bool            parseUri();
        bool            parseUriArgument();
        bool            parseProtocol();

        bool            parseOptions();
		bool			parseOption(const std::string&, HttpRequest::Option);
		bool            parseOptionHost();
	    bool            parseOptionContentLength();
		bool            parseOptionFrom();
		bool            parseOptionUserAgent();
		bool            parseOptionContentType();
		bool            parseOptionDate();

        bool            readEmailAddress(std::string&);
        bool            readHost(std::string&);
        bool            readAbsoluteUri(std::string&);
        bool            readRelativeUri(std::string&);
        bool            readDate(std::string&);
        bool            readTime(std::string&);
        bool            readUriParam(std::string&, std::string&);

        bool            isDayOfTheMonth(const int) const;
        bool            isDayOfTheWeek(const std::string&) const;
        bool            isMonthOfTheYear(const std::string&) const;
        bool            isHour(const int) const;
        bool            isMinute(const int) const;
        bool            isSecond(const int) const;

        

        HttpRequest*    _request;
        bool            _isFirstArgument;
        bool            _isFirstLine;
        bool            _isValid;
        bool            _isDone;
};

#endif /* !__HTTPPARSER_H__ */
