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
class IComponent;
class IInterfaceType;

/*!
	The co::Interface pseudo-interface, implicitly inherited by all Coral interfaces.
 */
class Interface
{
public:
	virtual ~Interface() {;}

	virtual IInterfaceType* getInterfaceType() = 0;

	virtual IComponent* getInterfaceOwner() = 0;

	virtual const std::string& getInterfaceName() = 0;

	virtual void componentRetain() = 0;

	virtual void componentRelease() = 0;
};

template<> struct kindOf<Interface> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<Interface> { static const char* get() { return "co.Interface"; } };
template<> struct typeOf<Interface> : public typeOfBase<Interface, IInterfaceType> {};

#define CORAL_DISAMBIGUATE_CO_INTERFACE( Super ) \
	co::IInterfaceType* getInterfaceType() = 0; \
	co::IComponent* getInterfaceOwner() = 0; \
	const std::string& getInterfaceName() = 0; \
	void componentRetain() = 0; \
	void componentRelease() = 0;

} // namespace co
	
#endif
