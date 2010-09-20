/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_COMPOUNDTYPE_H_
#define _CO_COMPOUNDTYPE_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Type.h>

// Forward Declarations:
namespace co {
	class MemberInfo;
} // namespace co
// End Of Forward Declarations

// co.CompoundType Mapping:
namespace co {

class CompoundType : public co::Type
{
public:
	virtual ~CompoundType() {;}

	virtual co::ArrayRange<co::MemberInfo* const> getMembers() = 0;

	virtual co::MemberInfo* getMember( const std::string& name ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::CompoundType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::CompoundType> { static const char* get() { return "co.CompoundType"; } };
template<> struct typeOf<co::CompoundType> : public typeOfBase<co::CompoundType, InterfaceType> {};
} // namespace co

#endif // _CO_COMPOUNDTYPE_H_
