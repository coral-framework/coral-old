/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _SERVICEMANAGER_H_
#define _SERVICEMANAGER_H_

#include "ServiceManager_Base.h"
#include <co/IComponent.h>
#include <map>

namespace co {

/*!
	Implements co.IServiceManager.
 */
class ServiceManager : public ServiceManager_Base
{
public:
	ServiceManager();
	virtual ~ServiceManager();

	// internal methods:
	void initialize();
	void tearDown();

	// IServiceManager methods:
	bool getIsLazy();
	void setIsLazy( bool isLazy );
	void addService( IInterface* serviceType, IService* serviceInstance );
	void addServiceForType( IInterface* serviceType, IInterface* clientType, IService* serviceInstance );
	void addServiceImplementation( IInterface* serviceType, const std::string& componentName );
	void addServiceImplementationForType( IInterface* serviceType, IInterface* clientType, const std::string& componentName );
	IService* getService( IInterface* serviceType );
	IService* getServiceForType( IInterface* serviceType, IInterface* clientType );
	IService* getServiceForInstance( IInterface* serviceType, IService* clientInstance );
	void removeService( IInterface* serviceType );
	void removeServiceForType( IInterface* serviceType, IInterface* clientType );

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

		inline IService* getInstance() { return _instance; }

		inline void setInstance( IService* instance )
		{
			clear();
			_instance = instance;
			_instance->componentRetain();
			_hasInstance = true;
		}

		inline IComponent* getComponentType() { return _implementation; }

		inline void setComponentType( IComponent* type )
		{
			clear();
			_implementation = type;
		}

	private:
		bool _hasInstance; // true if we got an '_instance'; false if we got an '_implementation'.
		union
		{
			IService* _instance;
			IComponent* _implementation;
		};
	};

	typedef std::map<IInterface*, LazyInstance> CustomServicesMap;

	struct ServiceRecord
	{
		LazyInstance global;
		CustomServicesMap custom; // specialized by clientType
	};

	typedef std::map<IInterface*, ServiceRecord> ServiceMap;

	inline ServiceRecord& fetchServiceRecord( IInterface* serviceType );
	inline IPort* getFacetInfo( IComponent* ct, IInterface* itfType );

	void validateClientType( IInterface* clientType );
	void validateServiceInstance( IInterface* serviceType, IService* serviceInstance );
	IComponent* validateComponentType( IInterface* serviceType, const std::string& componentName );
	
	void createServiceInstance( IInterface* serviceType, LazyInstance& instance, IInterface* clientType = 0 );

	IService* getServiceForType( ServiceRecord& rec, IInterface* serviceType, IInterface* clientType );
	LazyInstance* findSuitableInstance( ServiceRecord& rec, IInterface* serviceType, IInterface* clientType );

private:
	bool _isLazy;
	ServiceMap _services;
};

} // namespace co

#endif
