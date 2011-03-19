/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ICOMPONENT_H_
#define _CO_ICOMPONENT_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class IComponentType;
	class IInterfaceInfo;
} // namespace co
// End Of Forward Declarations

// co.IComponent Mapping:
namespace co {

class IComponent : public co::Interface
{
public:
	virtual ~IComponent() {;}

	// Code From <c++ Block:
	
		/*!
			<tt>getFacet<T>()</tt>: convenience method that returns the instance of a facet 'T' provided
			by this component. Please notice that this method only works for statically-defined components.
			It will not retrieve proxy interfaces from dynamic components (such as those implemented in Lua).
		 */
		template<typename T> inline T* getFacet() { return dynamic_cast<T*>( this ); }
	
	// End Of c++> Block

	virtual co::IComponentType* getComponentType() = 0;

	virtual co::Interface* getInterface( co::IInterfaceInfo* itfInfo ) = 0;

	virtual void setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IComponent> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IComponent> { static const char* get() { return "co.IComponent"; } };
template<> struct typeOf<co::IComponent> : public typeOfBase<co::IComponent, IInterfaceType> {};
} // namespace co

#endif // _CO_ICOMPONENT_H_
