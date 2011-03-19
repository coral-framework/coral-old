/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IDYNAMICPROXYHANDLER_H_
#define _CO_IDYNAMICPROXYHANDLER_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Any.h>
#include <vector>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class IAttributeInfo;
	class IMethodInfo;
} // namespace co
// End Of Forward Declarations

// co.IDynamicProxyHandler Mapping:
namespace co {

class IDynamicProxyHandler : public co::Interface
{
public:
	virtual ~IDynamicProxyHandler() {;}

	virtual const std::string& getProxyInterfaceName( co::int32 cookie ) = 0;

	virtual const co::Any& handleGetAttribute( co::int32 cookie, co::IAttributeInfo* ai ) = 0;

	virtual const co::Any& handleMethodInvocation( co::int32 cookie, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args ) = 0;

	virtual void handleSetAttribute( co::int32 cookie, co::IAttributeInfo* ai, const co::Any& value ) = 0;

	virtual co::int32 registerProxyInterface( co::Interface* proxy ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IDynamicProxyHandler> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IDynamicProxyHandler> { static const char* get() { return "co.IDynamicProxyHandler"; } };
template<> struct typeOf<co::IDynamicProxyHandler> : public typeOfBase<co::IDynamicProxyHandler, IInterfaceType> {};
} // namespace co

#endif // _CO_IDYNAMICPROXYHANDLER_H_
