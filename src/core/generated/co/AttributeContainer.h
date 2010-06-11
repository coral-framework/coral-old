/*******************************************************************************
** C++ mapping generated for type 'co.AttributeContainer'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_ATTRIBUTECONTAINER_H_
#define _CO_ATTRIBUTECONTAINER_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class AttributeInfo;
} // namespace co
// End Of Forward Declarations

// co.AttributeContainer Mapping:
namespace co {

class AttributeContainer : public co::Interface
{
public:
	virtual ~AttributeContainer() {;}

	virtual co::ArrayRange<co::AttributeInfo* const> getMemberAttributes() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::AttributeContainer> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::AttributeContainer> { static const char* get() { return "co.AttributeContainer"; } };
template<> struct typeOf<co::AttributeContainer> : public typeOfBase<co::AttributeContainer, InterfaceType> {};
} // namespace co

#endif // _CO_ATTRIBUTECONTAINER_H_
