/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ISERVICEMANAGER_H_
#define _CO_ISERVICEMANAGER_H_

#include <co/TypeTraits.h>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IInterface;
} // namespace co
// End Of Forward Declarations

// co.IServiceManager Mapping:
namespace co {

class IServiceManager : public co::IService
{
public:
	virtual ~IServiceManager() {;}

	virtual bool getIsLazy() = 0;

	virtual void setIsLazy( bool isLazy ) = 0;

	virtual void addService( co::IInterface* serviceType, co::IService* service ) = 0;

	virtual void addServiceForType( co::IInterface* serviceType, co::IInterface* clientType, co::IService* service ) = 0;

	virtual void addServiceProvider( co::IInterface* serviceType, const std::string& componentName ) = 0;

	virtual void addServiceProviderForType( co::IInterface* serviceType, co::IInterface* clientType, const std::string& componentName ) = 0;

	virtual co::IService* getService( co::IInterface* serviceType ) = 0;

	virtual co::IService* getServiceForInstance( co::IInterface* serviceType, co::IService* clientInstance ) = 0;

	virtual co::IService* getServiceForType( co::IInterface* serviceType, co::IInterface* clientType ) = 0;

	virtual void removeService( co::IInterface* serviceType ) = 0;

	virtual void removeServiceForType( co::IInterface* serviceType, co::IInterface* clientType ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IServiceManager> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IServiceManager> { static const char* get() { return "co.IServiceManager"; } };
template<> struct typeOf<co::IServiceManager> : public typeOfBase<co::IServiceManager, IInterface> {};
} // namespace co

#endif // _CO_ISERVICEMANAGER_H_
