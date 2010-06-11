/*******************************************************************************
** C++ mapping generated for type 'co.InterfaceInfo'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_INTERFACEINFO_H_
#define _CO_INTERFACEINFO_H_

#include <co/TypeTraits.h>
#include <co/MemberInfo.h>

// Forward Declarations:
namespace co {
	class InterfaceType;
} // namespace co
// End Of Forward Declarations

// co.InterfaceInfo Mapping:
namespace co {

class InterfaceInfo : public co::MemberInfo
{
public:
	virtual ~InterfaceInfo() {;}

	virtual bool getIsProvided() = 0;

	virtual co::InterfaceType* getType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::InterfaceInfo> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::InterfaceInfo> { static const char* get() { return "co.InterfaceInfo"; } };
template<> struct typeOf<co::InterfaceInfo> : public typeOfBase<co::InterfaceInfo, InterfaceType> {};
} // namespace co

#endif // _CO_INTERFACEINFO_H_
