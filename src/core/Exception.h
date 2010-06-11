/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_EXCEPTION_H_
#define _CO_EXCEPTION_H_

#include <co/TypeTraits.h>
#include <exception>

namespace co {

/*!
	Root class of the exception hierarchy of the Coral framework.
	All core and user exceptions inherit directly or indirectly from this class.
	\sa co::UserException
 */
class CORAL_EXPORT Exception : public std::exception
{
public:
	//! Constructs an Exception with the specified message.
	Exception( const std::string& message ) : _message( message )
	{;}

	//! Constructs a copy of an Exception.
	Exception( const Exception& other ) : std::exception()
	{
		_message = other._message;
	}

	//! Destructor
	virtual ~Exception() throw();

	//! Returns the exception message.
	const std::string& getMessage() const
	{
		return _message;
	}

	//! Updates the exception message.
	void setMessage( const std::string& message )
	{
		_message = message;
	}

	//! Returns the exception message.
	virtual const char* what() const throw();

private:
	// forbid copies
	Exception& operator=( const Exception& )
	{
		return *this;
	}

private:
	std::string _message;
};

template<> struct kindOf<Exception> : public kindOfBase<TK_EXCEPTION> {};

} // namespace co

/*!
	Macro to raise an exception with a std::ostream-style formatted message.
	You must #include <sstream> to use this.
 */
#define CORAL_THROW( exceptionType, message ) \
	{ \
		std::stringstream throwMessageStringStream; \
		throwMessageStringStream << message; \
		throw exceptionType( throwMessageStringStream.str() ); \
	}

#endif
