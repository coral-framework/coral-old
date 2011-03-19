/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ICOMPOUNDTYPE_H_
#define _CO_ICOMPOUNDTYPE_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/IType.h>

// Forward Declarations:
namespace co {
	class IMemberInfo;
} // namespace co
// End Of Forward Declarations

// co.ICompoundType Mapping:
namespace co {

class ICompoundType : public co::IType
{
public:
	virtual ~ICompoundType() {;}

	virtual co::ArrayRange<co::IMemberInfo* const> getMembers() = 0;

	virtual co::IMemberInfo* getMember( const std::string& name ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ICompoundType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ICompoundType> { static const char* get() { return "co.ICompoundType"; } };
template<> struct typeOf<co::ICompoundType> : public typeOfBase<co::ICompoundType, IInterfaceType> {};
} // namespace co

#endif // _CO_ICOMPOUNDTYPE_H_
