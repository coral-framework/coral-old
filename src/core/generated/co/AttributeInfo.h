/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ATTRIBUTEINFO_H_
#define _CO_ATTRIBUTEINFO_H_

#include <co/TypeTraits.h>
#include <co/MemberInfo.h>

// Forward Declarations:
namespace co {
	class Type;
} // namespace co
// End Of Forward Declarations

// co.AttributeInfo Mapping:
namespace co {

class AttributeInfo : public co::MemberInfo
{
public:
	virtual ~AttributeInfo() {;}

	virtual bool getIsReadOnly() = 0;

	virtual co::Type* getType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::AttributeInfo> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::AttributeInfo> { static const char* get() { return "co.AttributeInfo"; } };
template<> struct typeOf<co::AttributeInfo> : public typeOfBase<co::AttributeInfo, InterfaceType> {};
} // namespace co

#endif // _CO_ATTRIBUTEINFO_H_
