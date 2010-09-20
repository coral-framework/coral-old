/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_COMPONENT_H_
#define _CO_COMPONENT_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class ComponentType;
	class InterfaceInfo;
} // namespace co
// End Of Forward Declarations

// co.Component Mapping:
namespace co {

class Component : public co::Interface
{
public:
	virtual ~Component() {;}

	// Code From <c++ Block:
	
		/*!
			<tt>getProvided<T>()</tt>: convenience method that returns the instance of an interface 'T' provided
			by this component. Please notice that this method only works for statically-defined components.
			It will not retrieve proxy interfaces from dynamic components (such as those implemented in Lua).
		 */
		template<typename T> inline T* getProvided() { return dynamic_cast<T*>( this ); }
	
	// End Of c++> Block

	virtual co::ComponentType* getComponentType() = 0;

	virtual void bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance ) = 0;

	virtual co::Interface* getInterface( co::InterfaceInfo* interfaceInfo ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::Component> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::Component> { static const char* get() { return "co.Component"; } };
template<> struct typeOf<co::Component> : public typeOfBase<co::Component, InterfaceType> {};
} // namespace co

#endif // _CO_COMPONENT_H_
