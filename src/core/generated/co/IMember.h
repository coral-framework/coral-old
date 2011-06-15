/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IMEMBER_H_
#define _CO_IMEMBER_H_

#include <co/TypeTraits.h>
#include <co/IService.h>
#include <co/MemberKind.h>

// Forward Declarations:
namespace co {
	class ICompositeType;
} // namespace co
// End Of Forward Declarations

// co.IMember Mapping:
namespace co {

class IMember : public co::IService
{
public:
	virtual ~IMember() {;}

	virtual co::uint16 getIndex() = 0;

	virtual co::MemberKind getKind() = 0;

	virtual const std::string& getName() = 0;

	virtual co::ICompositeType* getOwner() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IMember> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IMember> { static const char* get() { return "co.IMember"; } };
template<> struct typeOf<co::IMember> : public typeOfBase<co::IMember, IInterface> {};
} // namespace co

#endif // _CO_IMEMBER_H_
