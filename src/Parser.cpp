#include <string>
#include <iostream>

#include "Workflow/Parser.h"

const int BUFF_SIZE = 256;
const char	COMMENT_CHAR = ';';

Parser::Parser(Buffer* buff) :  _i(0),
                    _backI(0),
                    _ignore(true), _comment(false),
                    _end(false), _lastReadChar(0),
                    _isFed(false)
{
    //TODO:Replace by our excpetion system
	/*if (extendBuffer() == false)
		throw Exception("Can't read file");*/
    if (buff == NULL)
    {
        _buffer = new Buffer(1024);
        _internalBuffer = true;
    }
    else
    {
        _buffer = buff;
        _internalBuffer = false;
        _isFed = true;
    }
}

Parser::~Parser()
{
    if (_internalBuffer)
        delete this->_buffer;
}

void        Parser::init()
{
    _lastReadChar = 0;
    _isFed = _internalBuffer == true;
    _end = false;
}

void		Parser::feed(const char* str, size_t size)
{
    this->_buffer->push(str, size);
    this->_end = false;
    this->_isFed = true;
}

bool        Parser::isFed() const
{
    return this->_buffer->size() > 0;
}

//bool		Parser::extendBuffer()
//{
//    //FIXME: voir pour fournir une classe de feeding, utilisee pour les modules et le parser.
//    // Si on n'a plus assez de donnees, on quite.
//    if (this->_bufferId >= (int)this->_buffers.size() - 1)
//    {
//        return false;
//    }
//    ++this->_bufferId;
//    this->_i = 0;
//    return true;
//}

char		Parser::peekChar()
{
	char	c;

	c = this->readChar();
    this->_lastReadChar = c;
	this->_i++;
	return (c);
}

bool        Parser::peekIfEqual(char toFind, std::string& target)
{
	this->ignore();
    if (this->readChar() == toFind)
    {
        target += this->peekChar();
		this->ignore();
        return true;
    }
    return false;
}

bool        Parser::peekIfNum(std::string& target)
{
    if (this->isNum(this->readChar()))
    {
        target += this->peekChar();
        return true;
    }
    return false;
}

bool        Parser::peekIfEqual(char toFind)
{
	this->ignore();
    if (this->readChar() == toFind)
    {
        this->peekChar();
		this->ignore();
        return true;
    }
    return false;
}

bool        Parser::peekIfEqual(const std::string& toFind, 
                                std::string& target)
{
    unsigned int     l = toFind.length();

    this->ignore();
	this->__saveContext();
    for (unsigned int i = 0; i < l; ++i)
    {
        if (this->peekIfEqual(toFind[i]) == false)
        {
            this->__restoreContext();
            return false;
        }
    }
	this->ignore();
    target += toFind;
    return true;
}

bool        Parser::peekIfEqual(const std::string& toFind) 
{
    unsigned int     l = toFind.length();

	this->ignore();
    this->__saveContext();
    for (unsigned int i = 0; i < l; ++i)
    {
        if (this->peekIfEqual(toFind[i]) == false)
        {
            this->__restoreContext();
            //std::cout << "false with " << this->readChar() << " / " << toFind[i] << std::endl;
            return false;
        }
    }
	this->ignore();
    return true;
}

std::string        Parser::peekNChar(int n)
{
    std::string     str;

    while (--n)
        str += this->peekChar();
    return str;
}

void		Parser::__saveContext()
{
	this->_backI = this->_i;
}

void        Parser::saveContextPub()
{
    this->_backIPub = this->_i;
}

void		Parser::__restoreContext()
{
	this->_i = this->_backI >= 0 ? this->_backI : 0;
}

void        Parser::restoreContextPub()
{
    this->_i = this->_backIPub >= 0 
                ? this->_backIPub : 0;
}

void		Parser::flush()
{
    this->_buffer->flush(this->_i);
    if (this->_i > 0)
        this->_i = 0;
}

char	Parser::readChar()
{
    //std::cout << "pre read char" << std::endl;
	char	c = this->_buffer->getChar(this->_i);
    //std::cout << "post read char" << std::endl;

    if (c < 0)
    {
        this->_end = true;
        return 0;
    }
    if (this->_comment)
    {
        this->_comment = false;
        this->skipComment(c);
        this->_comment = true;
    }
	return (c);
}

bool    Parser::readIfEqual(const std::string& toFind)
{
    int l = toFind.length();
    
    this->__saveContext(); 
    for (int i = 0; i < l; ++i, ++this->_i)
    {
        if (this->readChar() != toFind[i])
        {
            this->__restoreContext();
            return false;
        }
    }
    this->__restoreContext();
    return true;
}

void    Parser::readUpToIgnore()
{
    char c = this->readChar();
    while (c != '\n' && c != '\r' 
            && c != ' ' && c != '\t')
    {
        this->_i++;
        c = this->readChar();
    }
}

void    Parser::readUpToIgnore(std::string& output)
{
	char c = this->readChar();

	output = "";
	while (c != '\n' && c != '\r' 
		&& c != ' ' && c != '\t')
	{
		output += c;
		this->_i++;
        c = this->readChar();
	}
}


void    Parser::readUpTo(const std::string& end)
{
    char    c = this->readChar();

    while (true)
    {
        if (c == end[0])
        {
            if (this->readIfEqual(end) == true)
                break ;
        }
        this->_i++;
        c = this->readChar();
    }
}
void    Parser::readUpTo(const std::string& end, std::string& output)
{
    char    c = this->readChar();

    output = "";
    while (true)
    {
        if (c == end[0])
        {
            if (this->readIfEqual(end) == true)
                break ;
        }
        output += c;
        this->_i++;
        c = this->readChar();
    }
}

void	Parser::readUntilEndOfLine(std::string& output)
{
	output = "";
	this->setIgnore(false);
	while (true)
	{
		if (this->readIfEqual("\r\n"))
		{
			this->setIgnore(true);
			return ;
		}
		output += this->peekChar();
	}
}

void	Parser::readUntilEndOfLine()
{
	this->setIgnore(false);
	while (true)
	{
		if (this->readIfEqual("\r\n"))
		{
			this->setIgnore(true);
			return ;
		}
		this->peekChar();
	}
}

void    Parser::skipComment(char c)
{
    if (this->_commentList.find(c) == std::string::npos)
        return ;
    this->readUntilEndOfLine();
}

bool	Parser::isIgnore(char c) const
{
	return (c == ' ' 
            || c == '\t');
}

void	Parser::ignore()
{
    if (this->_ignore == false)
        return ;
	while (this->isIgnore(this->readChar()))
		this->peekChar();
}

bool	Parser::readIdentifier(std::string& output)
{
	std::string	identifier = "";
	char		c;

	this->ignore();
	c = this->readChar();
	if (this->isAlpha(c) || c == '_')
	{
		identifier += c;
        this->peekChar();
		c = this->readChar();
		while (this->isAlphaNum(c)
                || c == '_'
                || c == '-')
		{
			identifier += c;
            this->peekChar();
			c = this->readChar();
		}
		output = identifier;
        this->ignore();
		return true;
	}
	return false;
}

bool    Parser::appendIdentifier(std::string& target)
{
    std::string tmp;
    if (this->readIdentifier(tmp))
    {
        target += tmp;
        return true;
    }
    return false;
}

bool	Parser::readInteger(int& output)
{
	std::string		res("");
	char			c;

	this->ignore();
	this->__saveContext();
	c = this->peekChar();
	if (this->isNum(c))
	{
		while (this->isNum(c))
		{
			res += c;
			this->__saveContext();
			c = this->peekChar();
		}
		this->__restoreContext();
		output = atoi(res.c_str());
		this->ignore();
		return true;
	}
	this->__restoreContext();
	output = 0;
	return false;
}

bool	Parser::readInteger(std::string& output)
{
	std::string		res("");
	char			c;

	this->ignore();
	this->__saveContext();
	c = this->peekChar();
	if (this->isNum(c))
	{
		while (this->isNum(c))
		{
			res += c;
			this->__saveContext();
			c = this->peekChar();
		}
		this->__restoreContext();
		output = res;
		this->ignore();
		return true;
	}
	this->__restoreContext();
	output = "";
	return false;
}

bool	Parser::appendInteger(std::string& output)
{
	std::string	tmp;

	if (this->readInteger(tmp))
	{
		output += tmp;
		return true;
	}
	return false;
}

bool	Parser::readInteger(short& output)
{
	int		res;
	bool	ret;

	ret = this->readInteger(res);
	output = (short)res;
	return (ret);
}

bool	Parser::readDecimal(std::string& output)
{
	if (this->readInteger(output)
			&& this->peekIfEqual('.', output)
				&& this->appendInteger(output))
	{
		return true;
	}				
	output = "0.0";
	return false;
}

bool	Parser::appendDecimal(std::string& output)
{
	std::string		tmp;

	if (this->readDecimal(tmp))
	{
		output += tmp;
		return true;
	}
	return false;
}

bool	Parser::readDecimal(double& output)
{
	std::string		outStr;
	bool			ret;

	ret = this->readDecimal(outStr);
	output = atof(outStr.c_str());
	return ret;
}

bool	Parser::readDecimal(float& output)
{
	std::string		outStr;
	bool			ret;

	ret = this->readDecimal(outStr);
	output = (float)atof(outStr.c_str());
	return ret;
}

//bool	Parser::isEOF() const
//{
//	return (this->_stream.eof());
//}
//
//bool	Parser::isError() const
//{
//	return (this->_stream.bad() || this->_stream.fail());
//}
//

char    Parser::getLastReadChar() const
{
    return this->_lastReadChar;
}

bool    Parser::isEOL()
{
    return this->peekIfEqual("\r\n");
}

bool    Parser::hasEOL()
{
    bool    firstStep = false;
    char    c;

    this->__saveContext();
    while (!this->isEnd())
    {
        c = this->peekChar();
        if (c == '\r')
            firstStep = true;
        if (firstStep && c == '\n')
        {
            this->__restoreContext();
            return true;
        }
    }
    this->__restoreContext();
    return false;
}

bool    Parser::isEnd() const
{
    return this->_i >= this->_buffer->size();
}

bool    Parser::isAlpha()
{
    return this->isAlpha(this->readChar());
}

bool    Parser::isAlpha(const char c) const
{
    if ((c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z'))
        return true;
    return false;
}

bool    Parser::isNum()
{
    return this->isNum(this->readChar());
}

bool    Parser::isNum(const char c) const
{
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

bool    Parser::isAlphaNum()
{
    return this->isAlphaNum(this->readChar());
}

bool    Parser::isAlphaNum(const char c) const
{
    if (this->isAlpha(c) || this->isNum(c))
        return true;
    return false;
}

bool    Parser::getIgnore() const
{
    return this->_ignore;
}

void    Parser::setIgnore(bool ignore)
{
    this->_ignore = ignore;
}

bool    Parser::getComment() const
{
    return this->_comment;
}

void    Parser::setComment(bool comment)
{
    this->_comment = comment;
}

void    Parser::setCommentList(const char* list)
{
    if (list)
        this->_commentList = list;
}

bool	Parser::readAnythingBut(const std::string& forbiden, std::string& res)
{
	char	c;

	c = this->readChar();
	if (forbiden.find(c) == std::string::npos
			&& this->isIgnore(c) == false)
	{
		//fixme : I think a do while might do the trick, but I prefer not to try right now :D
		this->peekChar();
		res = c;
		c = this->readChar();
		while (forbiden.find(c) == std::string::npos
				&& this->isIgnore(c) == false
                && !this->_end)
		{
			this->peekChar();
			res += c;
			c = this->readChar();
		}
		return true;
	}
	return false;
}

bool	Parser::readAnythingBut(const std::string& forbiden)
{
	char	c;

	c = this->readChar();
	if (forbiden.find(c) == std::string::npos
			&& this->isIgnore(c) == false)
	{
		this->peekChar();
		c = this->readChar();
		while (forbiden.find(c) == std::string::npos
				&& this->isIgnore(c) == false
                && !this->_end)
		{
			this->peekChar();
			c = this->readChar();
		}
		return true;
	}
	return false;
}

void    Parser::dump() const
{
    this->_buffer->dump();
}

