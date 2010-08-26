/*******************************************************************************
** C++ mapping generated for type 'co.DynamicProxyHandler'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_DYNAMICPROXYHANDLER_H_
#define _CO_DYNAMICPROXYHANDLER_H_

#include <co/TypeTraits.h>
#include <co/Any.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class AttributeInfo;
	class MethodInfo;
} // namespace co
// End Of Forward Declarations

// co.DynamicProxyHandler Mapping:
namespace co {

class DynamicProxyHandler : public co::Interface
{
public:
	virtual ~DynamicProxyHandler() {;}

	virtual const std::string& getProxyInterfaceName( co::int32 cookie ) = 0;

	virtual void handleGetAttribute( co::int32 cookie, co::AttributeInfo* ai, co::Any& value ) = 0;

	virtual void handleMethodInvocation( co::int32 cookie, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& returnValue ) = 0;

	virtual void handleSetAttribute( co::int32 cookie, co::AttributeInfo* ai, const co::Any& value ) = 0;

	virtual co::int32 registerProxyInterface( co::Interface* proxy ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::DynamicProxyHandler> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::DynamicProxyHandler> { static const char* get() { return "co.DynamicProxyHandler"; } };
template<> struct typeOf<co::DynamicProxyHandler> : public typeOfBase<co::DynamicProxyHandler, InterfaceType> {};
} // namespace co

#endif // _CO_DYNAMICPROXYHANDLER_H_
