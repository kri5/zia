#ifndef __ZEXCEPTION_H__
# define __ZEXCEPTION_H__

#include <exception>
#include <string>
#include "Logger.hpp"

#define INFO __LINE__, __FILE__

template <class T>
class ZException : public std::exception
{
	public:
		ZException(int, const char*, typename T::Error::Code, const char*);
		virtual ~ZException() throw ();
		virtual const char*     what() const throw();

		std::string             getFile() const;
		std::string             getMessage() const;
		int                     getLine() const;
		typename T::Error::Code getCode() const;
	private:
		std::string             _file;
		std::string             _msg;
		int                     _line;
		typename T::Error::Code _code;
};


template <class T>
ZException<T>::ZException(int line, const char* file,
		typename T::Error::Code code = T::Error::Unknown,
		const char* msg = "") : 
	_line(line), _file(file), _code(code), _msg(msg)
{
	if (!_msg.length())
		_msg = T::Error::Msg[_code];

	Logger::getInstance() << Logger::Error
		<< "Exception thrown from file ("
		<< _file << ") at line " << _line
		<< " with message : " << _msg
		<< Logger::Flush;
}

	template <class T>
ZException<T>::~ZException() throw()
{

}

	template <class T>
const char*             ZException<T>::what() const throw()
{
	std::ostringstream  er;

	er << "Exception thrown from file (" << _file << ")"
		<< " at line " << _line 
		<< " with message : " << _msg << std::endl;

	return er.str().c_str();
}

template <class T>
std::string             ZException<T>::getFile() const
{
	return _file;
}

template <class T>
std::string             ZException<T>::getMessage() const
{
	return _msg;
}

template <class T>
int                     ZException<T>::getLine() const
{
	return _line;
}

template <class T>
typename T::Error::Code   ZException<T>::getCode() const
{
	return this->_code;
}

#endif /* !__ZEXCEPTION_H__ */
