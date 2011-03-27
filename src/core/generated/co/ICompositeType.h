/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ICOMPOSITETYPE_H_
#define _CO_ICOMPOSITETYPE_H_

#include <co/TypeTraits.h>
#include <co/IType.h>
#include <co/Range.h>

// Forward Declarations:
namespace co {
	class IMember;
} // namespace co
// End Of Forward Declarations

// co.ICompositeType Mapping:
namespace co {

class ICompositeType : public co::IType
{
public:
	virtual ~ICompositeType() {;}

	virtual co::Range<co::IMember* const> getMembers() = 0;

	virtual co::IMember* getMember( const std::string& name ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ICompositeType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ICompositeType> { static const char* get() { return "co.ICompositeType"; } };
template<> struct typeOf<co::ICompositeType> : public typeOfBase<co::ICompositeType, IInterface> {};
} // namespace co

#endif // _CO_ICOMPOSITETYPE_H_
