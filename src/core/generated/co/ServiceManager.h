/*******************************************************************************
** C++ mapping generated for type 'co.ServiceManager'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_SERVICEMANAGER_H_
#define _CO_SERVICEMANAGER_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class InterfaceType;
} // namespace co
// End Of Forward Declarations

// co.ServiceManager Mapping:
namespace co {

class ServiceManager : public co::Interface
{
public:
	virtual ~ServiceManager() {;}

	virtual bool getIsLazy() = 0;

	virtual void setIsLazy( bool isLazy ) = 0;

	virtual void addService( co::InterfaceType* serviceType, co::Interface* serviceInstance ) = 0;

	virtual void addServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType, co::Interface* serviceInstance ) = 0;

	virtual void addServiceImplementation( co::InterfaceType* serviceType, const std::string& componentName ) = 0;

	virtual void addServiceImplementationForType( co::InterfaceType* serviceType, co::InterfaceType* clientType, const std::string& componentName ) = 0;

	virtual co::Interface* getService( co::InterfaceType* serviceType ) = 0;

	virtual co::Interface* getServiceForInstance( co::InterfaceType* serviceType, co::Interface* clientInstance ) = 0;

	virtual co::Interface* getServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType ) = 0;

	virtual void removeService( co::InterfaceType* serviceType ) = 0;

	virtual void removeServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ServiceManager> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ServiceManager> { static const char* get() { return "co.ServiceManager"; } };
template<> struct typeOf<co::ServiceManager> : public typeOfBase<co::ServiceManager, InterfaceType> {};
} // namespace co

#endif // _CO_SERVICEMANAGER_H_
