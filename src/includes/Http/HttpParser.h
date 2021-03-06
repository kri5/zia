#ifndef __HTTPPARSER_H__
# define __HTTPPARSER_H__

#include "Workflow/Parser.h"
#include "Http/HttpRequest.h"

class HttpParser : public Parser
{
    public:
        HttpParser(HttpRequest*, Buffer* buff = NULL);
        virtual ~HttpParser();
        void            init();
        void            parse();
        bool            done() const;
        bool            isValid() const;
        bool            isFirstLine() const;
        HttpRequest*    getRequest() const;

    private:
        bool            parseGetCommand();
        bool            parsePostCommand();
        bool            parseHeadCommand();
        bool            parseGenericCommand();
        bool            parseUri();
        bool            parseUriArgument();
        bool            parseProtocol();

        bool            parseOptions();
		bool			parseOption(const std::string&, 
                                    std::string);

        bool            parseOptionAccept();
        bool            parseOptionConnection();
		bool            parseOptionHost();
	    bool            parseOptionContentLength();
		bool            parseOptionFrom();
		bool            parseOptionUserAgent();
		bool            parseOptionContentType();
		bool            parseOptionDate();
        bool            parseOptionGeneric();

        bool            readEmailAddress(std::string&);
        bool            readAcceptType(std::string&);
        bool            readAcceptParam(std::string&);
        bool            readHost(std::string&);
        bool            readAbsoluteUri(std::string&, bool&);
        bool            readRelativeUri(std::string&, bool&);
        bool            readDate(std::string&);
        bool            readTime(std::string&);
        bool            readParam(std::string&, std::string&);

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
