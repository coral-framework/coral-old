/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IDYNAMICSERVICEPROVIDER_H_
#define _CO_IDYNAMICSERVICEPROVIDER_H_

#include <co/TypeTraits.h>
#include <co/Any.h>
#include <co/IService.h>
#include <vector>
#include <co/Range.h>

// Forward Declarations:
namespace co {
	class IField;
	class IMethod;
} // namespace co
// End Of Forward Declarations

// co.IDynamicServiceProvider Mapping:
namespace co {

class IDynamicServiceProvider : public co::IService
{
public:
	virtual ~IDynamicServiceProvider() {;}

	virtual const std::string& getProxyInterfaceName( co::int32 cookie ) = 0;

	virtual const co::Any& handleGetAttribute( co::int32 cookie, co::IField* ai ) = 0;

	virtual const co::Any& handleMethodInvocation( co::int32 cookie, co::IMethod* mi, co::Range<co::Any const> args ) = 0;

	virtual void handleSetAttribute( co::int32 cookie, co::IField* ai, const co::Any& value ) = 0;

	virtual co::int32 registerProxyInterface( co::IService* proxy ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IDynamicServiceProvider> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IDynamicServiceProvider> { static const char* get() { return "co.IDynamicServiceProvider"; } };
template<> struct typeOf<co::IDynamicServiceProvider> : public typeOfBase<co::IDynamicServiceProvider, IInterface> {};
} // namespace co

#endif // _CO_IDYNAMICSERVICEPROVIDER_H_
