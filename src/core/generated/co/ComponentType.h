/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_COMPONENTTYPE_H_
#define _CO_COMPONENTTYPE_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/CompoundType.h>

// Forward Declarations:
namespace co {
	class InterfaceInfo;
} // namespace co
// End Of Forward Declarations

// co.ComponentType Mapping:
namespace co {

class ComponentType : public co::CompoundType
{
public:
	virtual ~ComponentType() {;}

	virtual co::ArrayRange<co::InterfaceInfo* const> getInterfaces() = 0;

	virtual co::ArrayRange<co::InterfaceInfo* const> getProvidedInterfaces() = 0;

	virtual co::ArrayRange<co::InterfaceInfo* const> getRequiredInterfaces() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ComponentType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ComponentType> { static const char* get() { return "co.ComponentType"; } };
template<> struct typeOf<co::ComponentType> : public typeOfBase<co::ComponentType, InterfaceType> {};
} // namespace co

#endif // _CO_COMPONENTTYPE_H_
