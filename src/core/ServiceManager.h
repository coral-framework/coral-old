/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _SERVICEMANAGER_H_
#define _SERVICEMANAGER_H_

#include "ServiceManagerComponent_Base.h"
#include <co/ComponentType.h>
#include <map>

/*!
	Component that implements co.ServiceManager.
 */
class ServiceManager : public co::ServiceManagerComponent_Base
{
public:
	ServiceManager();
	virtual ~ServiceManager();

	// internal methods:
	void initialize();
	void tearDown();

	// co::ServiceManager methods:
	bool getIsLazy();
	void setIsLazy( bool isLazy );
	void addService( co::InterfaceType* serviceType, co::Interface* serviceInstance );
	void addServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType, co::Interface* serviceInstance );
	void addServiceImplementation( co::InterfaceType* serviceType, const std::string& componentName );
	void addServiceImplementationForType( co::InterfaceType* serviceType, co::InterfaceType* clientType, const std::string& componentName );
	co::Interface* getService( co::InterfaceType* serviceType );
	co::Interface* getServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType );
	co::Interface* getServiceForInstance( co::InterfaceType* serviceType, co::Interface* clientInstance );
	void removeService( co::InterfaceType* serviceType );
	void removeServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType );

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

		inline co::Interface* getInstance() { return _instance; }

		inline void setInstance( co::Interface* instance )
		{
			clear();
			_instance = instance;
			_instance->componentRetain();
			_hasInstance = true;
		}

		inline co::ComponentType* getComponentType() { return _implementation; }

		inline void setComponentType( co::ComponentType* type )
		{
			clear();
			_implementation = type;
		}

	private:
		bool _hasInstance; // true if we got an '_instance'; false if we got an '_implementation'.
		union
		{
			co::Interface* _instance;
			co::ComponentType* _implementation;
		};
	};

	typedef std::map<co::InterfaceType*, LazyInstance> CustomServicesMap;

	struct ServiceRecord
	{
		LazyInstance global;
		CustomServicesMap custom; // specialized by clientType
	};

	typedef std::map<co::InterfaceType*, ServiceRecord> ServiceMap;

	inline ServiceRecord& fetchServiceRecord( co::InterfaceType* serviceType );
	inline co::InterfaceInfo* getProvidedInterfaceInfo( co::ComponentType* ct, co::InterfaceType* itfType );

	void validateClientType( co::InterfaceType* clientType );
	void validateServiceInstance( co::InterfaceType* serviceType, co::Interface* serviceInstance );
	co::ComponentType* validateComponentType( co::InterfaceType* serviceType, const std::string& componentName );
	
	void createServiceInstance( co::InterfaceType* serviceType, LazyInstance& instance, co::InterfaceType* clientType = 0 );

	co::Interface* getServiceForType( ServiceRecord& rec, co::InterfaceType* serviceType, co::InterfaceType* clientType );
	LazyInstance* findSuitableInstance( ServiceRecord& rec, co::InterfaceType* serviceType, co::InterfaceType* clientType );

private:
	bool _isLazy;
	ServiceMap _services;
};

#endif
