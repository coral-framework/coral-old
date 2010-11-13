/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_EXCEPTIONTYPECOMPONENT_BASE_H_
#define _CO_EXCEPTIONTYPECOMPONENT_BASE_H_

#include <co/ExceptionType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ExceptionTypeComponent has a facet named 'type', of type co.ExceptionType.
class ExceptionTypeComponent_co_ExceptionType : public co::ExceptionType
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ExceptionTypeComponent'.
 */
class ExceptionTypeComponent_Base : public co::ComponentBase,
	public ExceptionTypeComponent_co_ExceptionType
{
public:
	ExceptionTypeComponent_Base();
	virtual ~ExceptionTypeComponent_Base();

	// co::Interface Methods:
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_EXCEPTIONTYPECOMPONENT_BASE_H_
