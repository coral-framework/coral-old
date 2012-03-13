/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IDYNAMICSERVICEPROVIDER_H_
#define _CO_IDYNAMICSERVICEPROVIDER_H_

#include <co/TypeTraits.h>
#include <co/Any.h>
#include <co/Range.h>
#include <vector>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IField;
	class IMethod;
	class IPort;
} // namespace co
// End Of Forward Declarations

// co.IDynamicServiceProvider Mapping:
namespace co {

class IDynamicServiceProvider : public co::IService
{
public:
	virtual ~IDynamicServiceProvider() {;}

	virtual co::IPort* dynamicGetFacet( co::int32 dynFacetId ) = 0;

	virtual co::Any dynamicGetField( co::int32 dynFacetId, co::IField* field ) = 0;

	virtual co::Any dynamicInvoke( co::int32 dynFacetId, co::IMethod* method, co::Range<co::Any> args ) = 0;

	virtual co::int32 dynamicRegisterService( co::IService* dynamicServiceProxy ) = 0;

	virtual void dynamicSetField( co::int32 dynFacetId, co::IField* field, co::Any value ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IDynamicServiceProvider> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IDynamicServiceProvider> { static const char* get() { return "co.IDynamicServiceProvider"; } };
} // namespace co

#endif // _CO_IDYNAMICSERVICEPROVIDER_H_
