/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IMETHODCONTAINER_H_
#define _CO_IMETHODCONTAINER_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class IMethodInfo;
} // namespace co
// End Of Forward Declarations

// co.IMethodContainer Mapping:
namespace co {

class IMethodContainer : public co::Interface
{
public:
	virtual ~IMethodContainer() {;}

	virtual co::ArrayRange<co::IMethodInfo* const> getMemberMethods() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IMethodContainer> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IMethodContainer> { static const char* get() { return "co.IMethodContainer"; } };
template<> struct typeOf<co::IMethodContainer> : public typeOfBase<co::IMethodContainer, IInterfaceType> {};
} // namespace co

#endif // _CO_IMETHODCONTAINER_H_
