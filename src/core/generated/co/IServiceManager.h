/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ISERVICEMANAGER_H_
#define _CO_ISERVICEMANAGER_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class IInterfaceType;
} // namespace co
// End Of Forward Declarations

// co.IServiceManager Mapping:
namespace co {

class IServiceManager : public co::Interface
{
public:
	virtual ~IServiceManager() {;}

	virtual bool getIsLazy() = 0;

	virtual void setIsLazy( bool isLazy ) = 0;

	virtual void addService( co::IInterfaceType* serviceType, co::Interface* serviceInstance ) = 0;

	virtual void addServiceForType( co::IInterfaceType* serviceType, co::IInterfaceType* clientType, co::Interface* serviceInstance ) = 0;

	virtual void addServiceImplementation( co::IInterfaceType* serviceType, const std::string& componentName ) = 0;

	virtual void addServiceImplementationForType( co::IInterfaceType* serviceType, co::IInterfaceType* clientType, const std::string& componentName ) = 0;

	virtual co::Interface* getService( co::IInterfaceType* serviceType ) = 0;

	virtual co::Interface* getServiceForInstance( co::IInterfaceType* serviceType, co::Interface* clientInstance ) = 0;

	virtual co::Interface* getServiceForType( co::IInterfaceType* serviceType, co::IInterfaceType* clientType ) = 0;

	virtual void removeService( co::IInterfaceType* serviceType ) = 0;

	virtual void removeServiceForType( co::IInterfaceType* serviceType, co::IInterfaceType* clientType ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IServiceManager> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IServiceManager> { static const char* get() { return "co.IServiceManager"; } };
template<> struct typeOf<co::IServiceManager> : public typeOfBase<co::IServiceManager, IInterfaceType> {};
} // namespace co

#endif // _CO_ISERVICEMANAGER_H_
