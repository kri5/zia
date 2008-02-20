#include <string>
#include <iostream>

#include "Parser.h"

const int BUFF_SIZE = 256;
const char	COMMENT_CHAR = ';';

Parser::Parser()
{
	_i = 0;
	_bufferId = -1;
	_backI = 0;
	_backBuffer = 0;
    //TODO:Replace by our excpetion system
	/*if (extendBuffer() == false)
		throw Exception("Can't read file");*/
}

bool		Parser::extendBuffer()
{
//	if (this->_i)
//		std::cout << "Extending buffer : " << this->_buffers[this->_bufferId][this->_i - 1] << std::endl;
	if ((int)this->_buffers.size() - 1 > this->_bufferId)
	{
		//std::cout << this->_bufferId << " // " << this->_buffers.size() << std::endl;
		this->_bufferId++;
		this->_i = 0;
		//std::cout << "Extended buffer : " << this->_buffers[this->_bufferId][this->_i] << std::endl;
		return true;
	}
	if (this->_stream.good())
	{
		char	buff[BUFF_SIZE + 1];

		this->_stream.get(buff, BUFF_SIZE + 1, 0);
		this->_nbRead = this->_stream.gcount();
		if (this->_nbRead <= 0)
			return false;
//		std::cout << "Adding buffer " << this->_bufferId + 1 << '(' << buff[0] << buff[1] << ") " << std::endl;
		this->_buffers.push_back(buff);
		this->_bufferId++;
		this->_i = 0;
//		std::cout << "Extended buffer : " << this->_buffers[this->_bufferId][this->_i] << std::endl;
		return true;
	}
	return false;
}

char		Parser::peekChar()
{
	char	c;

	if (this->_i >= this->_nbRead|| this->_bufferId < 0)
		this->extendBuffer();
	c = this->_buffers[this->_bufferId][this->_i];
	this->_i++;
	//std::cout << "peekChar : [" << (int)c << "] i = " << this->_i << std::endl;
	return (c);
}

void		Parser::saveContext()
{
	this->_backBuffer = this->_bufferId;
	this->_backI = this->_i;
}

void        Parser::saveContextPub()
{
    this->_backBufferPub = this->_bufferId;
    this->_backIPub = this->_i;
}

void		Parser::restoreContext()
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

//	std::cout << "flushing" << std::endl;
//	std::cout << "Current buffer : " << this->_bufferId << " " << this->_i << std::endl;
	for (i = this->_bufferId - 1; i >= 0; --i)
	{
//		std::cout << "erasing buffer :" << i << std::endl;
		this->_buffers.erase(this->_buffers.begin());
		this->_bufferId--;
	}
	if (this->_i == this->_nbRead)
	{
//		std::cout << "Flushing first buffer" << std::endl;
		this->_buffers.erase(this->_buffers.begin());
		this->_bufferId--;
//		std::cout << this->_buffers.size() << std::endl;
	}
}

char	Parser::readChar()
{
	char	c;

	//if (this->_i >= this->_nbRead || this->_bufferId < 0)
        //TODO: Replace by our Exception System
		//if (!this->extendBuffer())
		//	throw Exception("Can't read anymore");
	c = this->_buffers[this->_bufferId][this->_i];
	//std::cout << "readChar : [" << c << ']' << std::endl;
	return (c);
}

void	Parser::ignore()
{
	char	c;

	//std::cout << "Before ignore : [" << (int)this->_buffers[this->_bufferId][this->_i] << ']' << std::endl;
	while ((c = this->readChar()) == ' ' 
            || c == '\t' || c == '\n' 
            || c == '\r')
	{
		this->peekChar();
	}
	//std::cout << "After ignore : " << &this->_buffers[this->_bufferId][this->_i] << std::endl;
}

bool	Parser::readIdentifier(std::string& output)
{
	std::string	identifier = "";
	char		c;

	this->ignore();
	this->saveContext();
	c = this->peekChar();
	//std::cout << "Begining readIdentifier" << c << std::endl;
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') 
            || (c >= '0' && c <= '9')
            || c == '/' || c == '.')
	{
		identifier += c;
		c = this->peekChar();
		while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') 
                || (c >= '0' && c <= '9') 
                || c == '_' || c == '-' 
                || c == '.' || c == '/')
		{
			identifier += c;
			this->saveContext();
			c = this->peekChar();
		}
		this->restoreContext();
		output = identifier;
		this->ignore();
		return true;
	}
    //std::cout << "Exiting readIdentifier failing" << std::endl;
	this->restoreContext();
	return false;
}

bool	Parser::readInteger(int& output)
{
	std::string		res("");
	char			c;

	this->ignore();
	this->saveContext();
	c = this->peekChar();
	if (c >= '0' && c <= '9')
	{
		while (c >= '0' && c <= '9')
		{
			res += c;
			this->saveContext();
			c = this->peekChar();
		}
		this->restoreContext();
		output = atoi(res.c_str());
		this->ignore();
		return true;
	}
	this->restoreContext();
	output = 0;
	return false;
}

bool	Parser::readInteger(std::string& output)
{
	std::string		res("");
	char			c;

	this->ignore();
	this->saveContext();
	c = this->peekChar();
	if (c >= '0' && c <= '9')
	{
		while (c >= '0' && c <= '9')
		{
			res += c;
			this->saveContext();
			c = this->peekChar();
		}
		this->restoreContext();
		output = res;
		this->ignore();
		return true;
	}
	this->restoreContext();
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
	if ((c >= '0' && c <= '9') || c == '.')
	{
		c = this->peekChar();
		while ((c >= '0' && c <= '9') || c == '.')
		{
			res += c;
			this->saveContext();
			c = this->peekChar();
		}
		this->restoreContext();
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

bool	Parser::isEOF() const
{
	return (this->_stream.eof());
}

bool	Parser::isError() const
{
	return (this->_stream.bad() || this->_stream.fail());
}
