/*******************************************************************************
** C++ mapping generated for type 'co.NativeClassType'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_NATIVECLASSTYPE_H_
#define _CO_NATIVECLASSTYPE_H_

#include <co/TypeTraits.h>
#include <co/AttributeContainer.h>
#include <co/CompoundType.h>
#include <co/MethodContainer.h>

// co.NativeClassType Mapping:
namespace co {

class NativeClassType : public co::CompoundType, public co::AttributeContainer, public co::MethodContainer
{
public:
	virtual ~NativeClassType() {;}

	virtual const std::string& getHeaderName() = 0;

	virtual const std::string& getNativeName() = 0;

	CORAL_DISAMBIGUATE_CO_INTERFACE( co::CompoundType );
};

} // namespace co

namespace co {
template<> struct kindOf<co::NativeClassType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::NativeClassType> { static const char* get() { return "co.NativeClassType"; } };
template<> struct typeOf<co::NativeClassType> : public typeOfBase<co::NativeClassType, InterfaceType> {};
namespace traits {
	template<> struct hasAmbiguousBase<co::NativeClassType, co::Interface> : public TrueType {};
	template<> struct disambiguateBase<co::NativeClassType, co::Interface> { typedef co::CompoundType Super; };
} // namespace traits
} // namespace co

#endif // _CO_NATIVECLASSTYPE_H_
