/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IENUMTYPE_H_
#define _CO_IENUMTYPE_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/IType.h>

// co.IEnumType Mapping:
namespace co {

class IEnumType : public co::IType
{
public:
	virtual ~IEnumType() {;}

	virtual co::ArrayRange<std::string const> getIdentifiers() = 0;

	virtual co::int32 getValueOf( const std::string& identifier ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IEnumType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IEnumType> { static const char* get() { return "co.IEnumType"; } };
template<> struct typeOf<co::IEnumType> : public typeOfBase<co::IEnumType, IInterfaceType> {};
} // namespace co

#endif // _CO_IENUMTYPE_H_
