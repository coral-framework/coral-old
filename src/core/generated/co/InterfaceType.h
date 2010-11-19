/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INTERFACETYPE_H_
#define _CO_INTERFACETYPE_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/CompoundType.h>
#include <co/AttributeContainer.h>
#include <co/MethodContainer.h>
#include <co/InterfaceType.h>

// co.InterfaceType Mapping:
namespace co {

class InterfaceType : public co::CompoundType, public co::AttributeContainer, public co::MethodContainer
{
public:
	virtual ~InterfaceType() {;}

	virtual const std::string& getCppBlock() = 0;

	virtual co::ArrayRange<co::InterfaceType* const> getInterfaceAncestors() = 0;

	virtual co::ArrayRange<co::InterfaceType* const> getSubInterfaces() = 0;

	virtual co::ArrayRange<co::InterfaceType* const> getSuperInterfaces() = 0;

	virtual bool isSubTypeOf( co::InterfaceType* itf ) = 0;

	CORAL_DISAMBIGUATE_CO_INTERFACE( co::CompoundType );
};

} // namespace co

namespace co {
template<> struct kindOf<co::InterfaceType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::InterfaceType> { static const char* get() { return "co.InterfaceType"; } };
template<> struct typeOf<co::InterfaceType> : public typeOfBase<co::InterfaceType, InterfaceType> {};
namespace traits {
	template<> struct hasAmbiguousBase<co::InterfaceType, co::Interface> : public TrueType {};
	template<> struct disambiguateBase<co::InterfaceType, co::Interface> { typedef co::CompoundType Super; };
} // namespace traits
} // namespace co

#endif // _CO_INTERFACETYPE_H_
