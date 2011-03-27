/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_EXCEPTIONTYPE_BASE_H_
#define _CO_EXCEPTIONTYPE_BASE_H_

#include <co/IException.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ExceptionType has a facet named 'type', of type co.IException.
class ExceptionType_co_IException : public co::IException
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ExceptionType'.
 */
class ExceptionType_Base : public co::ComponentBase,
	public ExceptionType_co_IException
{
public:
	ExceptionType_Base();
	virtual ~ExceptionType_Base();

	// co::IService Methods:
	co::IObject* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IObject Methods:
	co::IComponent* getComponentType();
	co::IService* getInterface( co::IPort* );
	void setReceptacle( co::IPort*, co::IService* );
};

} // namespace co

#endif // _CO_EXCEPTIONTYPE_BASE_H_
