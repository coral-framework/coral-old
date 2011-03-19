/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ISTRUCTTYPE_H_
#define _CO_ISTRUCTTYPE_H_

#include <co/TypeTraits.h>
#include <co/IAttributeContainer.h>
#include <co/ICompoundType.h>

// co.IStructType Mapping:
namespace co {

class IStructType : public co::ICompoundType, public co::IAttributeContainer
{
public:
	virtual ~IStructType() {;}

	CORAL_DISAMBIGUATE_CO_INTERFACE( co::ICompoundType );
};

} // namespace co

namespace co {
template<> struct kindOf<co::IStructType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IStructType> { static const char* get() { return "co.IStructType"; } };
template<> struct typeOf<co::IStructType> : public typeOfBase<co::IStructType, IInterfaceType> {};
namespace traits {
	template<> struct hasAmbiguousBase<co::IStructType, co::Interface> : public TrueType {};
	template<> struct disambiguateBase<co::IStructType, co::Interface> { typedef co::ICompoundType Super; };
} // namespace traits
} // namespace co

#endif // _CO_ISTRUCTTYPE_H_
