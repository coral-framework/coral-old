/*******************************************************************************
** Base class generated for component 'co.ExceptionTypeComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_EXCEPTIONTYPECOMPONENT_BASE_H_
#define _CO_EXCEPTIONTYPECOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/ExceptionType.h>

namespace co {

//! co.ExceptionTypeComponent provides an interface named 'type', of type co.ExceptionType
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
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_EXCEPTIONTYPECOMPONENT_BASE_H_
