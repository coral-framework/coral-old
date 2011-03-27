/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INTERFACE_H_
#define _CO_INTERFACE_H_

#include <co/TypeTraits.h>

namespace co {

USING_CORAL_INTEGER_TYPES;

// Forward declarations:
class IObject;
class IInterface;

/*!
	The co::IService interface, implicitly inherited by all Coral interfaces.
 */
class IService
{
public:
	virtual ~IService() {;}

	virtual IInterface* getInterfaceType() = 0;

	virtual IObject* getInterfaceOwner() = 0;

	virtual const std::string& getInterfaceName() = 0;

	virtual void componentRetain() = 0;

	virtual void componentRelease() = 0;
};

template<> struct kindOf<IService> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<IService> { static const char* get() { return "co.IService"; } };
template<> struct typeOf<IService> : public typeOfBase<IService, IInterface> {};

#define CORAL_DISAMBIGUATE_CO_INTERFACE( Super ) \
	co::IInterface* getInterfaceType() = 0; \
	co::IObject* getInterfaceOwner() = 0; \
	const std::string& getInterfaceName() = 0; \
	void componentRetain() = 0; \
	void componentRelease() = 0;

} // namespace co

#endif
