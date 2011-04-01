/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ISERVICE_H_
#define _CO_ISERVICE_H_

#include <co/TypeTraits.h>

namespace co {

USING_CORAL_INTEGER_TYPES;

// Forward Decls:
class IPort;
class IObject;
class IInterface;

/*!
	The co.IService interface, implicitly inherited by all interfaces.
 */
class IService
{
public:
	virtual ~IService() {;}

	virtual IInterface* getInterface() = 0;

	virtual IObject* getProvider() = 0;

	virtual IPort* getFacet() = 0;

	virtual void serviceRetain() = 0;

	virtual void serviceRelease() = 0;
};

template<> struct kindOf<IService> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<IService> { static const char* get() { return "co.IService"; } };
template<> struct typeOf<IService> : public typeOfBase<IService, IInterface> {};

} // namespace co

#endif
