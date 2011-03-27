/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IENUM_H_
#define _CO_IENUM_H_

#include <co/TypeTraits.h>
#include <co/IType.h>
#include <co/Range.h>

// co.IEnum Mapping:
namespace co {

class IEnum : public co::IType
{
public:
	virtual ~IEnum() {;}

	virtual co::Range<std::string const> getIdentifiers() = 0;

	virtual co::int32 getValueOf( const std::string& identifier ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IEnum> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IEnum> { static const char* get() { return "co.IEnum"; } };
template<> struct typeOf<co::IEnum> : public typeOfBase<co::IEnum, IInterface> {};
} // namespace co

#endif // _CO_IENUM_H_
