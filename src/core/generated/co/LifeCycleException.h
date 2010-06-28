/*******************************************************************************
** C++ mapping generated for type 'co.LifeCycleException'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_LIFECYCLEEXCEPTION_H_
#define _CO_LIFECYCLEEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.LifeCycleException Mapping:
namespace co {

class LifeCycleException : public co::UserException
{
public:
	LifeCycleException()
	{;}
	
	LifeCycleException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::LifeCycleException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::LifeCycleException> { static const char* get() { return "co.LifeCycleException"; } };
template<> struct typeOf<co::LifeCycleException> : public typeOfBase<co::LifeCycleException, ExceptionType> {};
} // namespace co

#endif // _CO_LIFECYCLEEXCEPTION_H_
