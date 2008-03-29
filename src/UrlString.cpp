#include <string>
#include <cctype>
#include "Utils/UrlString.h"

std::string     UrlString::charToHex(char c)
{
    std::string out;
    char first, second;
    
    first = (c & 0xF0) / 16;
    first += first > 9 ? 'A' - 10 : '0';
    second = c & 0x0F;
    second += second > 9 ? 'A' - 10 : '0';

    out.append(1, first);
    out.append(1, second);

    return out;
}

char            UrlString::hexToChar(char first, char second)
{
    int digit;

    digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0'));
    digit *= 16;
    digit += (second >= 'A' ? ((second & 0xDF) - 'A') + 10 : (second - '0'));
    return static_cast<char>(digit);
}

bool            UrlString::mustConvert(char c)
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57))
        return false;
    switch (c)
    {
        case '/': case '+': case '-': case '_':
        case '?': case '$': case '@': case '\'':
        case '*': case '(': case ')': case '=':
        case ':': case '~': case '|': case '.':
        case '!': case '#':
            return false;
        default:
            return true;
    }
    return true;
}

std::string     UrlString::urlEncode(std::string source)
{
    std::string out;
    std::string::iterator it = source.begin();

    for (; it < source.end(); ++it)
    {
        if (mustConvert(*it))
            out += "%" + charToHex(*it);
        else
            out += *it;
    }
    return out;
}

std::string     UrlString::urlDecode(std::string source)
{
    std::string out;
    std::string::iterator it = source.begin();
    char c;

    for (; it < source.end(); ++it)
    {
        if (*it == '%')
        {
            if (std::distance(it, source.end()) >= 2 &&
            std::isxdigit(*(it + 1)) &&
            std::isxdigit(*(it + 2)))
            {
                c = *++it;     
                out.append(1, hexToChar(c, *++it));
                continue;
            } 
        }
        out += *it;
    }
    return out;
}

