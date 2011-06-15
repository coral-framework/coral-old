/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_METHOD_BASE_H_
#define _CO_METHOD_BASE_H_

#include <co/IMethod.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.Method has a facet named 'methodInfo', of type co.IMethod.
class Method_co_IMethod : public co::IMethod
{
public:
	virtual co::IInterface* getInterface();
	virtual co::IPort* getFacet();
};

/*!
	Inherit from this class to implement the component 'co.Method'.
 */
class Method_Base : public co::ComponentBase,
	public Method_co_IMethod
{
public:
	Method_Base();
	virtual ~Method_Base();

	// co::IService Methods:
	co::IObject* getProvider();
	void serviceRetain();
	void serviceRelease();

	// co::IObject Methods:
	co::IComponent* getComponent();
	co::IService* getService( co::IPort* );
	void setService( co::IPort*, co::IService* );
};

} // namespace co

#endif // _CO_METHOD_BASE_H_
