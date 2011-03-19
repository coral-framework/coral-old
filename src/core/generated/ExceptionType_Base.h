/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_EXCEPTIONTYPE_BASE_H_
#define _CO_EXCEPTIONTYPE_BASE_H_

#include <co/IExceptionType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ExceptionType has a facet named 'type', of type co.IExceptionType.
class ExceptionType_co_IExceptionType : public co::IExceptionType
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ExceptionType'.
 */
class ExceptionType_Base : public co::ComponentBase,
	public ExceptionType_co_IExceptionType
{
public:
	ExceptionType_Base();
	virtual ~ExceptionType_Base();

	// co::Interface Methods:
	co::IComponent* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IComponent Methods:
	co::IComponentType* getComponentType();
	co::Interface* getInterface( co::IInterfaceInfo* );
	void setReceptacle( co::IInterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_EXCEPTIONTYPE_BASE_H_
