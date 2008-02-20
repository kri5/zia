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

    private:
        bool            parseGetCommand();
        bool            parsePostCommand();
        bool            parseUri();
        bool            parseUriArgument();
        bool            parseProtocol();

        bool            parseOption();
        bool            parseOptionHost();
        bool            parseOptionContentLength();
        bool            parseOptionFrom();
        bool            parseOptionUserAgent();
        bool            parseOptionContentType();
        bool            parseOptionDate();

        bool            readEmailAddress(std::string&);
        bool            readDate(std::string&);
        bool            readTime(std::string&);

        bool            isWeekOfTheDay(const std::string&) const;
        bool            isMonthOfTheYear(const std::string&) const;

        HttpRequest*    _request;
};

#endif /* !__HTTPPARSER_H__ */
