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
        bool            parseCommand();
        bool            parseUri();
        bool            parseOption();
        bool            parseProtocol();

    private:
        HttpRequest*    _request;
};

#endif /* !__HTTPPARSER_H__ */
