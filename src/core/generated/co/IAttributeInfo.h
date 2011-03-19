/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IATTRIBUTEINFO_H_
#define _CO_IATTRIBUTEINFO_H_

#include <co/TypeTraits.h>
#include <co/IMemberInfo.h>

// Forward Declarations:
namespace co {
	class IType;
} // namespace co
// End Of Forward Declarations

// co.IAttributeInfo Mapping:
namespace co {

class IAttributeInfo : public co::IMemberInfo
{
public:
	virtual ~IAttributeInfo() {;}

	virtual bool getIsReadOnly() = 0;

	virtual co::IType* getType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IAttributeInfo> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IAttributeInfo> { static const char* get() { return "co.IAttributeInfo"; } };
template<> struct typeOf<co::IAttributeInfo> : public typeOfBase<co::IAttributeInfo, IInterfaceType> {};
} // namespace co

#endif // _CO_IATTRIBUTEINFO_H_
