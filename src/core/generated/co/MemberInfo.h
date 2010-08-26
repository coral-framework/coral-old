/*******************************************************************************
** C++ mapping generated for type 'co.MemberInfo'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_MEMBERINFO_H_
#define _CO_MEMBERINFO_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class CompoundType;
} // namespace co
// End Of Forward Declarations

// co.MemberInfo Mapping:
namespace co {

class MemberInfo : public co::Interface
{
public:
	virtual ~MemberInfo() {;}

	virtual co::uint32 getIndex() = 0;

	virtual const std::string& getName() = 0;

	virtual co::CompoundType* getOwner() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::MemberInfo> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::MemberInfo> { static const char* get() { return "co.MemberInfo"; } };
template<> struct typeOf<co::MemberInfo> : public typeOfBase<co::MemberInfo, InterfaceType> {};
} // namespace co

#endif // _CO_MEMBERINFO_H_
