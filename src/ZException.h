#ifndef __ZEXCEPTION_H__
# define __ZEXCEPTION_H__

#include <exception>
#include <string>

#define ZEXCEPTION(msg) ZException(__LINE__, __FILE__, msg)

class ZException : public std::exception
{
    public:
        ZException(int, const char*, const char*);
        virtual ~ZException() throw ();
        virtual const char*   what() const throw();

        std::string     getFile();
        int             getLine();
    private:
        std::string     _file;
        std::string     _msg;
        int             _line;
};

#endif /* !__ZEXCEPTION_H__ */
