/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IINTERFACEINFO_H_
#define _CO_IINTERFACEINFO_H_

#include <co/TypeTraits.h>
#include <co/IMemberInfo.h>

// Forward Declarations:
namespace co {
	class IInterfaceType;
} // namespace co
// End Of Forward Declarations

// co.IInterfaceInfo Mapping:
namespace co {

class IInterfaceInfo : public co::IMemberInfo
{
public:
	virtual ~IInterfaceInfo() {;}

	virtual bool getIsFacet() = 0;

	virtual co::IInterfaceType* getType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IInterfaceInfo> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IInterfaceInfo> { static const char* get() { return "co.IInterfaceInfo"; } };
template<> struct typeOf<co::IInterfaceInfo> : public typeOfBase<co::IInterfaceInfo, IInterfaceType> {};
} // namespace co

#endif // _CO_IINTERFACEINFO_H_
