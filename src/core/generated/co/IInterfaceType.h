/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IINTERFACETYPE_H_
#define _CO_IINTERFACETYPE_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/IMethodContainer.h>
#include <co/IAttributeContainer.h>
#include <co/ICompoundType.h>
#include <co/IInterfaceType.h>

// co.IInterfaceType Mapping:
namespace co {

class IInterfaceType : public co::ICompoundType, public co::IAttributeContainer, public co::IMethodContainer
{
public:
	virtual ~IInterfaceType() {;}

	virtual const std::string& getCppBlock() = 0;

	virtual co::ArrayRange<co::IInterfaceType* const> getInterfaceAncestors() = 0;

	virtual co::ArrayRange<co::IInterfaceType* const> getSubInterfaces() = 0;

	virtual co::ArrayRange<co::IInterfaceType* const> getSuperInterfaces() = 0;

	virtual bool isSubTypeOf( co::IInterfaceType* itf ) = 0;

	CORAL_DISAMBIGUATE_CO_INTERFACE( co::ICompoundType );
};

} // namespace co

namespace co {
template<> struct kindOf<co::IInterfaceType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IInterfaceType> { static const char* get() { return "co.IInterfaceType"; } };
template<> struct typeOf<co::IInterfaceType> : public typeOfBase<co::IInterfaceType, IInterfaceType> {};
namespace traits {
	template<> struct hasAmbiguousBase<co::IInterfaceType, co::Interface> : public TrueType {};
	template<> struct disambiguateBase<co::IInterfaceType, co::Interface> { typedef co::ICompoundType Super; };
} // namespace traits
} // namespace co

#endif // _CO_IINTERFACETYPE_H_
