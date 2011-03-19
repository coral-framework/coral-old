/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ICOMPONENTTYPE_H_
#define _CO_ICOMPONENTTYPE_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/ICompoundType.h>

// Forward Declarations:
namespace co {
	class IInterfaceInfo;
} // namespace co
// End Of Forward Declarations

// co.IComponentType Mapping:
namespace co {

class IComponentType : public co::ICompoundType
{
public:
	virtual ~IComponentType() {;}

	virtual co::ArrayRange<co::IInterfaceInfo* const> getFacets() = 0;

	virtual co::ArrayRange<co::IInterfaceInfo* const> getInterfaces() = 0;

	virtual co::ArrayRange<co::IInterfaceInfo* const> getReceptacles() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IComponentType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IComponentType> { static const char* get() { return "co.IComponentType"; } };
template<> struct typeOf<co::IComponentType> : public typeOfBase<co::IComponentType, IInterfaceType> {};
} // namespace co

#endif // _CO_ICOMPONENTTYPE_H_
