/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _SERVICEMANAGER_H_
#define _SERVICEMANAGER_H_

#include "ServiceManager_Base.h"
#include <co/IComponentType.h>
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
	void addService( IInterfaceType* serviceType, Interface* serviceInstance );
	void addServiceForType( IInterfaceType* serviceType, IInterfaceType* clientType, Interface* serviceInstance );
	void addServiceImplementation( IInterfaceType* serviceType, const std::string& componentName );
	void addServiceImplementationForType( IInterfaceType* serviceType, IInterfaceType* clientType, const std::string& componentName );
	Interface* getService( IInterfaceType* serviceType );
	Interface* getServiceForType( IInterfaceType* serviceType, IInterfaceType* clientType );
	Interface* getServiceForInstance( IInterfaceType* serviceType, Interface* clientInstance );
	void removeService( IInterfaceType* serviceType );
	void removeServiceForType( IInterfaceType* serviceType, IInterfaceType* clientType );

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

		inline IComponentType* getComponentType() { return _implementation; }

		inline void setComponentType( IComponentType* type )
		{
			clear();
			_implementation = type;
		}

	private:
		bool _hasInstance; // true if we got an '_instance'; false if we got an '_implementation'.
		union
		{
			Interface* _instance;
			IComponentType* _implementation;
		};
	};

	typedef std::map<IInterfaceType*, LazyInstance> CustomServicesMap;

	struct ServiceRecord
	{
		LazyInstance global;
		CustomServicesMap custom; // specialized by clientType
	};

	typedef std::map<IInterfaceType*, ServiceRecord> ServiceMap;

	inline ServiceRecord& fetchServiceRecord( IInterfaceType* serviceType );
	inline IInterfaceInfo* getFacetInfo( IComponentType* ct, IInterfaceType* itfType );

	void validateClientType( IInterfaceType* clientType );
	void validateServiceInstance( IInterfaceType* serviceType, Interface* serviceInstance );
	IComponentType* validateComponentType( IInterfaceType* serviceType, const std::string& componentName );
	
	void createServiceInstance( IInterfaceType* serviceType, LazyInstance& instance, IInterfaceType* clientType = 0 );

	Interface* getServiceForType( ServiceRecord& rec, IInterfaceType* serviceType, IInterfaceType* clientType );
	LazyInstance* findSuitableInstance( ServiceRecord& rec, IInterfaceType* serviceType, IInterfaceType* clientType );

private:
	bool _isLazy;
	ServiceMap _services;
};

} // namespace co

#endif
