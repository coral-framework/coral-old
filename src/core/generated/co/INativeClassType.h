/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INATIVECLASSTYPE_H_
#define _CO_INATIVECLASSTYPE_H_

#include <co/TypeTraits.h>
#include <co/IAttributeContainer.h>
#include <co/ICompoundType.h>
#include <co/IMethodContainer.h>

// co.INativeClassType Mapping:
namespace co {

class INativeClassType : public co::ICompoundType, public co::IAttributeContainer, public co::IMethodContainer
{
public:
	virtual ~INativeClassType() {;}

	virtual const std::string& getNativeHeaderFile() = 0;

	virtual const std::string& getNativeName() = 0;

	CORAL_DISAMBIGUATE_CO_INTERFACE( co::ICompoundType );
};

} // namespace co

namespace co {
template<> struct kindOf<co::INativeClassType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::INativeClassType> { static const char* get() { return "co.INativeClassType"; } };
template<> struct typeOf<co::INativeClassType> : public typeOfBase<co::INativeClassType, IInterfaceType> {};
namespace traits {
	template<> struct hasAmbiguousBase<co::INativeClassType, co::Interface> : public TrueType {};
	template<> struct disambiguateBase<co::INativeClassType, co::Interface> { typedef co::ICompoundType Super; };
} // namespace traits
} // namespace co

#endif // _CO_INATIVECLASSTYPE_H_
