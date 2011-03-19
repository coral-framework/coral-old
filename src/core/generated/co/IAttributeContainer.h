/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IATTRIBUTECONTAINER_H_
#define _CO_IATTRIBUTECONTAINER_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class IAttributeInfo;
} // namespace co
// End Of Forward Declarations

// co.IAttributeContainer Mapping:
namespace co {

class IAttributeContainer : public co::Interface
{
public:
	virtual ~IAttributeContainer() {;}

	virtual co::ArrayRange<co::IAttributeInfo* const> getMemberAttributes() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IAttributeContainer> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IAttributeContainer> { static const char* get() { return "co.IAttributeContainer"; } };
template<> struct typeOf<co::IAttributeContainer> : public typeOfBase<co::IAttributeContainer, IInterfaceType> {};
} // namespace co

#endif // _CO_IATTRIBUTECONTAINER_H_
