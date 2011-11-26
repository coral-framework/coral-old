/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ICOMPONENT_H_
#define _CO_ICOMPONENT_H_

#include <co/TypeTraits.h>
#include <co/Range.h>
#include <co/ICompositeType.h>

// Forward Declarations:
namespace co {
	class IPort;
} // namespace co
// End Of Forward Declarations

// co.IComponent Mapping:
namespace co {

class IComponent : public co::ICompositeType
{
public:
	virtual ~IComponent() {;}

	virtual co::Range<co::IPort* const> getFacets() = 0;

	virtual co::Range<co::IPort* const> getPorts() = 0;

	virtual co::Range<co::IPort* const> getReceptacles() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IComponent> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IComponent> { static const char* get() { return "co.IComponent"; } };
} // namespace co

#endif // _CO_ICOMPONENT_H_
