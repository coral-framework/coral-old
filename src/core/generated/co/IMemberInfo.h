/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IMEMBERINFO_H_
#define _CO_IMEMBERINFO_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class ICompoundType;
} // namespace co
// End Of Forward Declarations

// co.IMemberInfo Mapping:
namespace co {

class IMemberInfo : public co::Interface
{
public:
	virtual ~IMemberInfo() {;}

	virtual co::uint16 getIndex() = 0;

	virtual const std::string& getName() = 0;

	virtual co::ICompoundType* getOwner() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IMemberInfo> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IMemberInfo> { static const char* get() { return "co.IMemberInfo"; } };
template<> struct typeOf<co::IMemberInfo> : public typeOfBase<co::IMemberInfo, IInterfaceType> {};
} // namespace co

#endif // _CO_IMEMBERINFO_H_
