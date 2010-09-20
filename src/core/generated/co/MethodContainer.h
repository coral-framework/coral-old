/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_METHODCONTAINER_H_
#define _CO_METHODCONTAINER_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class MethodInfo;
} // namespace co
// End Of Forward Declarations

// co.MethodContainer Mapping:
namespace co {

class MethodContainer : public co::Interface
{
public:
	virtual ~MethodContainer() {;}

	virtual co::ArrayRange<co::MethodInfo* const> getMemberMethods() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::MethodContainer> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::MethodContainer> { static const char* get() { return "co.MethodContainer"; } };
template<> struct typeOf<co::MethodContainer> : public typeOfBase<co::MethodContainer, InterfaceType> {};
} // namespace co

#endif // _CO_METHODCONTAINER_H_
