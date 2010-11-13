/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ENUMTYPE_H_
#define _CO_ENUMTYPE_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Type.h>

// co.EnumType Mapping:
namespace co {

class EnumType : public co::Type
{
public:
	virtual ~EnumType() {;}

	virtual co::ArrayRange<std::string const> getIdentifiers() = 0;

	virtual co::int32 getValueOf( const std::string& identifier ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::EnumType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::EnumType> { static const char* get() { return "co.EnumType"; } };
template<> struct typeOf<co::EnumType> : public typeOfBase<co::EnumType, InterfaceType> {};
} // namespace co

#endif // _CO_ENUMTYPE_H_
