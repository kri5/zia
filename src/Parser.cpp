#include <string>
#include <iostream>

#include "Parser.h"

const int BUFF_SIZE = 256;
const char	COMMENT_CHAR = ';';

Parser::Parser() :  _i(0), _bufferId(-1),
                    _backI(0), _backBuffer(0),
                    _ignore(true), _comment(false)
{
    //TODO:Replace by our excpetion system
	/*if (extendBuffer() == false)
		throw Exception("Can't read file");*/
}

bool		Parser::extendBuffer()
{
	if ((int)this->_buffers.size() - 1 > this->_bufferId)
	{
		this->_bufferId++;
		this->_i = 0;
		return true;
	}
	if (this->_stream.good())
	{
		char	buff[BUFF_SIZE + 1];

		this->_stream.get(buff, BUFF_SIZE + 1, 0);
		this->_nbRead = this->_stream.gcount();
		if (this->_nbRead <= 0)
			return false;
		this->_buffers.push_back(buff);
		this->_bufferId++;
		this->_i = 0;
		return true;
	}
	return false;
}

char		Parser::peekChar()
{
	char	c;

	c = this->readChar();
	this->_i++;
	return (c);
}

bool        Parser::peekIfEqual(char toFind, std::string& target)
{
    if (this->readChar() == toFind)
    {
        target += this->peekChar();
        return true;
    }
    return false;
}

bool        Parser::peekIfEqual(char toFind)
{
    if (this->readChar() == toFind)
    {
        this->peekChar();
        return true;
    }
    return false;
}

bool        Parser::peekIfEqual(const std::string& toFind, 
                                std::string& target)
{
    unsigned int     l = toFind.length();

    this->__saveContext();
    for (unsigned int i = 0; i < l; ++i)
    {
        if (!this->peekIfEqual(toFind[i]))
        {
            this->__restoreContext();
            return false;
        }
    }
    target += toFind;
    return true;
}

bool        Parser::peekIfEqual(const std::string& toFind) 
{
    unsigned int     l = toFind.length();

    this->__saveContext();
    for (unsigned int i = 0; i < l; ++i)
    {
        if (this->peekIfEqual(toFind[i]))
        {
            this->__restoreContext();
            return false;
        }
    }
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
	this->_backBuffer = this->_bufferId;
	this->_backI = this->_i;
}

void        Parser::saveContextPub()
{
    this->_backBufferPub = this->_bufferId;
    this->_backIPub = this->_i;
}

void		Parser::__restoreContext()
{
	this->_i = this->_backI >= 0 ? this->_backI : 0;
	this->_bufferId = this->_backBuffer >= 0 ? this->_backBuffer : 0;
}

void        Parser::restoreContextPub()
{
    this->_i = this->_backIPub >= 0 
                ? this->_backIPub : 0;

    this->_bufferId = this->_backBufferPub >= 0 
                        ? this->_backBufferPub : 0;
}

void		Parser::flush()
{
	int		i;

	for (i = this->_bufferId - 1; i >= 0; --i)
	{
		this->_buffers.erase(this->_buffers.begin());
		this->_bufferId--;
	}
	if (this->_i == this->_nbRead)
	{
		this->_buffers.erase(this->_buffers.begin());
		this->_bufferId--;
	}
}

char	Parser::readChar()
{
	char	c;

	if (this->_i >= this->_nbRead || this->_bufferId < 0)
        //TODO: Replace by our Exception System
		if (!this->extendBuffer())
			throw "Can't read anymore";
	c = this->_buffers[this->_bufferId][this->_i];
    if (this->_comment)
        this->skipComment(c);
	return (c);
}

void    Parser::readUpToIgnore()
{
    char c = this->_buffers[this->_bufferId][this->_i];
    while (c != '\n' && c != '\r' 
            && c != ' ' && c != '\t')
    {
        this->_i++;
        if (this->_i >= this->_nbRead || this->_bufferId < 0)
            this->extendBuffer();
        c = this->_buffers[this->_bufferId][this->_i];
    }
}

void    Parser::skipComment(char c)
{
    if (this->_commentList.find(c) == std::string::npos)
        return ;
    this->readUpToIgnore();
}

void	Parser::ignore()
{
	char	c;

    if (this->_ignore == false)
        return ;
	while ((c = this->readChar()) == ' ' 
            || c == '\t' || c == '\n' 
            || c == '\r')
	{
		this->peekChar();
	}
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
                || c == '_')
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
bool	Parser::readInteger(short& output)
{
	int		res;
	bool	ret;

	ret = this->readInteger(res);
	output = (short)res;
	return (ret);
}

bool	Parser::readDecimal(double& output)
{
	std::string		res("");
	char			c;

	this->ignore();
	c = this->readChar();
	if (this->isNum(c) || c == '.')
	{
		c = this->peekChar();
		while (this->isNum(c) || c == '.')
		{
			res += c;
			this->__saveContext();
			c = this->peekChar();
		}
		this->__restoreContext();
		output = atof(res.c_str());
		this->ignore();
		return true;
	}
	output = .0f;
	return false;
}

bool	Parser::readDecimal(float& output)
{
	double	res;
	bool	ret;

	ret = this->readDecimal(res);
	output = (float)res;
	return (ret);
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

void    Parser::printI() const
{
    std::cout << "[Parser] this->_i == " << this->_i << std::endl;
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
