/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_EXCEPTION_H_
#define _CO_EXCEPTION_H_

#include <co/TypeTraits.h>
#include <exception>

namespace co {

/*!
	\brief Base class for all Coral exceptions.
 */
class CORAL_EXPORT Exception : public std::exception
{
public:
	//! Constructs an Exception with an empty message.
	inline Exception()
	{;}

	//! Constructs an Exception with the specified message.
	inline Exception( const std::string& message ) : _message( message )
	{;}

	//! Constructs a copy of an Exception.
	inline Exception( const Exception& other )
		: std::exception(), _message( other._message )
	{;}

	//! Destructor.
	virtual ~Exception() throw();

	//! Returns the exception message.
	inline const std::string& getMessage() const { return _message; }

	//! Sets the exception message.
	inline void setMessage( const std::string& message ) { _message = message; }

	//! Returns the exception message as C-string, for compatibility with std::exception.
	virtual const char* what() const throw();

private:
	// copies are forbidden
	Exception& operator=( const Exception& );

private:
	std::string _message;
};

#ifndef DOXYGEN
template<> struct kindOf<Exception> : public kindOfBase<TK_EXCEPTION> {};
#endif // DOXYGEN

} // namespace co

/*!
	Macro to raise an exception with a std::ostream-style formatted message.
	You must #include \<sstream\> to use this macro.
	\relates co::Exception
 */
#define CORAL_THROW( exceptionType, message ) \
	{ \
		std::stringstream throwMessageStringStream; \
		throwMessageStringStream << message; \
		throw exceptionType( throwMessageStringStream.str() ); \
	}

#endif
