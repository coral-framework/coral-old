/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _COMPONENTTYPE_H_
#define _COMPONENTTYPE_H_

#include "Type.h"
#include "ComponentTypeComponent_Base.h"
#include <co/RefVector.h>
#include <co/InterfaceInfo.h>

/*!
	Component that implements co.ComponentType.
 */
class ComponentType : public co::ComponentTypeComponent_Base, public TypeImpl
{
public:
	ComponentType();
	virtual ~ComponentType();

	// internal methods:

	//! Appends the given interfaces range to the interfaces list. After calling this
	//! method the interfaces list must be re-sorted using the sortInterfaces() method.
	void addInterfaces( co::ArrayRange<co::InterfaceInfo* const> interfaces );

	//! Sort the interfaces list by (provided, name) and update _firstProvidedPos.
	void sortInterfaces();
	
	// co::CompoundType methods:
	co::ArrayRange<co::MemberInfo* const> getMembers();
	co::MemberInfo* getMember( const std::string& name );

	// co::ComponentType methods:
	co::ArrayRange<co::InterfaceInfo* const> getInterfaces();
	co::ArrayRange<co::InterfaceInfo* const> getRequiredInterfaces();
	co::ArrayRange<co::InterfaceInfo* const> getProvidedInterfaces();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );

private:
	typedef co::RefVector<co::InterfaceInfo> InterfacesVector;
	InterfacesVector _interfaces;	// dual sorted vector: client interfaces first, then server interfaces.
	std::size_t _firstProvidedPos;	// dividing point: position of the first provided interface in _interfaces.
};

#endif
