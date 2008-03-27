#ifndef URLSTRING_H
#define URLSTRING_H

#include <string>

class UrlString
{
public:
    static std::string  urlDecode(std::string source);
    static std::string  urlEncode(std::string source);

private:
    static bool         mustConvert(char c);
    static std::string  charToHex(char c);
    static char         hexToChar(char first, char second);
};

#endif // URLSTRING_H
