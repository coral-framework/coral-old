/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _SERVICEMANAGER_H_
#define _SERVICEMANAGER_H_

#include "ServiceManagerComponent_Base.h"
#include <co/ComponentType.h>
#include <map>

namespace co {

/*!
	Component that implements co.ServiceManager.
 */
class ServiceManagerComponent : public ServiceManagerComponent_Base
{
public:
	ServiceManagerComponent();
	virtual ~ServiceManagerComponent();

	// internal methods:
	void initialize();
	void tearDown();

	// ServiceManager methods:
	bool getIsLazy();
	void setIsLazy( bool isLazy );
	void addService( InterfaceType* serviceType, Interface* serviceInstance );
	void addServiceForType( InterfaceType* serviceType, InterfaceType* clientType, Interface* serviceInstance );
	void addServiceImplementation( InterfaceType* serviceType, const std::string& componentName );
	void addServiceImplementationForType( InterfaceType* serviceType, InterfaceType* clientType, const std::string& componentName );
	Interface* getService( InterfaceType* serviceType );
	Interface* getServiceForType( InterfaceType* serviceType, InterfaceType* clientType );
	Interface* getServiceForInstance( InterfaceType* serviceType, Interface* clientInstance );
	void removeService( InterfaceType* serviceType );
	void removeServiceForType( InterfaceType* serviceType, InterfaceType* clientType );

private:
	class LazyInstance
	{
	public:
		inline LazyInstance() : _hasInstance( false ), _instance( NULL ) {;}
		inline ~LazyInstance() { clear(); }

		inline bool isValid() const { return _instance != NULL; }

		inline void clear()
		{
			if( _hasInstance )
				_instance->componentRelease();
			_instance = NULL;
			_hasInstance = false;
		}

		inline bool hasInstance() const { return _hasInstance; }

		inline Interface* getInstance() { return _instance; }

		inline void setInstance( Interface* instance )
		{
			clear();
			_instance = instance;
			_instance->componentRetain();
			_hasInstance = true;
		}

		inline ComponentType* getComponentType() { return _implementation; }

		inline void setComponentType( ComponentType* type )
		{
			clear();
			_implementation = type;
		}

	private:
		bool _hasInstance; // true if we got an '_instance'; false if we got an '_implementation'.
		union
		{
			Interface* _instance;
			ComponentType* _implementation;
		};
	};

	typedef std::map<InterfaceType*, LazyInstance> CustomServicesMap;

	struct ServiceRecord
	{
		LazyInstance global;
		CustomServicesMap custom; // specialized by clientType
	};

	typedef std::map<InterfaceType*, ServiceRecord> ServiceMap;

	inline ServiceRecord& fetchServiceRecord( InterfaceType* serviceType );
	inline InterfaceInfo* getFacetInfo( ComponentType* ct, InterfaceType* itfType );

	void validateClientType( InterfaceType* clientType );
	void validateServiceInstance( InterfaceType* serviceType, Interface* serviceInstance );
	ComponentType* validateComponentType( InterfaceType* serviceType, const std::string& componentName );
	
	void createServiceInstance( InterfaceType* serviceType, LazyInstance& instance, InterfaceType* clientType = 0 );

	Interface* getServiceForType( ServiceRecord& rec, InterfaceType* serviceType, InterfaceType* clientType );
	LazyInstance* findSuitableInstance( ServiceRecord& rec, InterfaceType* serviceType, InterfaceType* clientType );

private:
	bool _isLazy;
	ServiceMap _services;
};

} // namespace co

#endif
