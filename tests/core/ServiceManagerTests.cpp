/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/Coral.h>
#include <co/ISystem.h>
#include <co/IArrayType.h>
#include <co/IComponent.h>
#include <co/IMemberInfo.h>
#include <co/IStructType.h>
#include <co/ITypeManager.h>
#include <co/IInterfaceType.h>
#include <co/IServiceManager.h>
#include <co/IAttributeContainer.h>
#include <co/ITypeCreationTransaction.h>
#include <co/MissingServiceException.h>
#include <co/IllegalArgumentException.h>
#include <co/NoSuchInterfaceException.h>
#include <gtest/gtest.h>

TEST( ServiceManagerTests, globalService )
{
	co::IServiceManager* sm = co::getSystem()->getServices();

	// try to get the ITypeManager service before it's been added
	co::IInterfaceType* typeManagerType = co::typeOf<co::ITypeManager>::get();
	EXPECT_THROW( sm->getService( NULL ), co::IllegalArgumentException );
	EXPECT_THROW( sm->getService( typeManagerType ), co::MissingServiceException );

	// add the ITypeManager service
	EXPECT_THROW( sm->addService( typeManagerType, NULL ), co::IllegalArgumentException );
	EXPECT_THROW( sm->addService( typeManagerType, co::getSystem() ), co::IllegalArgumentException );
	sm->addService( typeManagerType, co::getSystem()->getTypes() );

	// now we should be able to get it
	EXPECT_TRUE( sm->getService( typeManagerType ) != NULL );

	// test the utility functions
	EXPECT_EQ( sm->getService( typeManagerType ), co::getServiceForType( typeManagerType, NULL ) );
	EXPECT_EQ( sm->getService( typeManagerType ), co::getService<co::ITypeManager>() );

	// remove the service & check if it's been removed
	sm->removeService( typeManagerType );
	EXPECT_THROW( sm->removeService( typeManagerType ), co::MissingServiceException );
	EXPECT_THROW( sm->getService( typeManagerType ), co::MissingServiceException );
}

TEST( ServiceManagerTests, lazyGlobalService )
{
	co::IServiceManager* sm = co::getSystem()->getServices();

	// make sure the service has not been added yet
	co::IInterfaceType* tctType = co::typeOf<co::ITypeCreationTransaction>::get();
	EXPECT_THROW( sm->getService( tctType ), co::MissingServiceException );

	// add the service with lazy instantiation
	EXPECT_TRUE( sm->getIsLazy() );
	EXPECT_THROW( sm->addServiceImplementation( NULL, "co.TypeCreationTransaction" ),
					co::IllegalArgumentException );
	EXPECT_THROW( sm->addServiceImplementation( tctType, "invalid" ), co::IllegalArgumentException );
	EXPECT_THROW( sm->addServiceImplementation( co::typeOf<co::IType>::get(),
					"co.TypeCreationTransaction" ), co::NoSuchInterfaceException );
	sm->addServiceImplementation( tctType, "co.TypeCreationTransaction" );

	// now we should be able to get the service instance
	co::Interface* service = sm->getService( tctType );
	EXPECT_TRUE( service != NULL );
	EXPECT_EQ( tctType, service->getInterfaceType() );

	// remove the service
	co::ITypeCreationTransaction* tct = dynamic_cast<co::ITypeCreationTransaction*>( service );
	ASSERT_TRUE( tct != NULL );

	tct->rollback();
	sm->removeService( tctType );

	EXPECT_THROW( sm->getService( tctType ), co::MissingServiceException );
}

TEST( ServiceManagerTests, customServicesPerType )
{
	co::IServiceManager* sm = co::getSystem()->getServices();

	// make sure the service has not been added yet
	co::IInterfaceType* memberInfoType = co::typeOf<co::IMemberInfo>::get();
	EXPECT_THROW( sm->getService( memberInfoType ), co::MissingServiceException );

	// add a custom instance for AttributeContainers
	co::IInterfaceType* attribContType = co::typeOf<co::IAttributeContainer>::get();
	co::IComponent* component = co::newInstance( "co.AttributeInfo" );
	sm->addServiceForType( memberInfoType, attribContType, component->getFacet<co::IMemberInfo>() );

	// add a different custom instance for InterfaceTypes
	co::IInterfaceType* interfaceTypeType = co::typeOf<co::IInterfaceType>::get();
	sm->addServiceImplementationForType( memberInfoType, interfaceTypeType, "co.InterfaceInfo" );

	// getting the service for an IArrayType should hit an unavailable global instance
	EXPECT_THROW( sm->getServiceForType( memberInfoType, co::typeOf<co::IArrayType>::get() ),
					co::MissingServiceException );

	// getting the service for a IStructType should return an IAttributeInfo
	co::Interface* itf = sm->getServiceForType( memberInfoType, co::typeOf<co::IStructType>::get() );
	EXPECT_EQ( "co.IAttributeInfo", itf->getInterfaceType()->getFullName() );

	// getting the service for an IInterfaceType should return an IInterfaceInfo
	itf = sm->getServiceForType( memberInfoType, interfaceTypeType );
	EXPECT_EQ( "co.IInterfaceInfo", itf->getInterfaceType()->getFullName() );

	// test the utility functions
	EXPECT_EQ( itf, co::getServiceForType( memberInfoType, interfaceTypeType ) );
	EXPECT_EQ( itf, co::getService<co::IMemberInfo>( interfaceTypeType ) );

	// add a global instance for the service
	sm->addServiceImplementation( memberInfoType, "co.MethodInfo" );

	// now, getting the service for an IArrayType should return a IMethodInfo
	co::IInterfaceType* arrayTypeType = co::typeOf<co::IArrayType>::get();
	itf = sm->getServiceForType( memberInfoType, arrayTypeType );
	EXPECT_EQ( "co.IMethodInfo", itf->getInterfaceType()->getFullName() );

	// test the utility functions
	EXPECT_EQ( itf, co::getServiceForType( memberInfoType, arrayTypeType ) );
	EXPECT_EQ( itf, co::getService<co::IMemberInfo>( arrayTypeType ) );

	// getting the service for a IStructType should still return an IAttributeInfo
	itf = sm->getServiceForType( memberInfoType, co::typeOf<co::IStructType>::get() );
	EXPECT_EQ( "co.IAttributeInfo", itf->getInterfaceType()->getFullName() );

	// getting the service for an IInterfaceType should still return an IInterfaceInfo
	itf = sm->getServiceForType( memberInfoType, interfaceTypeType );
	EXPECT_EQ( "co.IInterfaceInfo", itf->getInterfaceType()->getFullName() );
}

TEST( ServiceManagerTests, customServicesPerInstance )
{
	// --- assumes the services added in 'customServicesPerType' have not been removed ---
	co::IServiceManager* sm = co::getSystem()->getServices();
	co::IInterfaceType* memberInfoType = co::typeOf<co::IMemberInfo>::get();

	// getting a IMemberInfo service for a IMethodInfo instance should return the intance's interface
	co::IMemberInfo* aMethod = co::typeOf<co::Interface>::get()->getMember( "componentRetain" );
	ASSERT_TRUE( aMethod != NULL );

	co::Interface* itf = sm->getServiceForInstance( memberInfoType, aMethod );
	co::IMemberInfo* mi = dynamic_cast<co::IMemberInfo*>( itf );
	ASSERT_TRUE( mi != NULL );
	EXPECT_EQ( "componentRetain", mi->getName() );

	EXPECT_EQ( itf, co::getServiceForInstance( memberInfoType, aMethod ) );
	EXPECT_EQ( itf, co::getService<co::IMemberInfo>( aMethod ) );

	// getting a IMemberInfo service for the system should return the global service instance
	itf = sm->getServiceForInstance( memberInfoType, co::getSystem() );
	EXPECT_EQ( "co.IMethodInfo", itf->getInterfaceType()->getFullName() );

	EXPECT_EQ( itf, co::getServiceForInstance( memberInfoType, co::getSystem() ) );
	EXPECT_EQ( itf, co::getService<co::IMemberInfo>( co::getSystem() ) );

	/*
		Getting a IMemberInfo service for an IInterfaceType instance should return
			the IInterfaceType-specialized service instance.
	 */
	co::Interface* clientInstance = co::disambiguate<co::Interface>( co::typeOf<co::IType>::get() );
	itf = sm->getServiceForInstance( memberInfoType, clientInstance );
	EXPECT_EQ( "co.IInterfaceInfo", itf->getInterfaceType()->getFullName() );

	EXPECT_EQ( itf, co::getServiceForInstance( memberInfoType, clientInstance ) );
	EXPECT_EQ( itf, co::getService<co::IMemberInfo>( clientInstance ) );
}

TEST( ServiceManagerTests, serviceRemovals )
{
	// --- assumes the services added in 'customServicesPerType' have not been removed ---
	co::IServiceManager* sm = co::getSystem()->getServices();
	co::IInterfaceType* memberInfoType = co::typeOf<co::IMemberInfo>::get();

	// remove the service's global instance
	sm->removeService( memberInfoType );

	// getting the service for an IArrayType should hit an unavailable global instance
	EXPECT_THROW( sm->getServiceForType( memberInfoType, co::typeOf<co::IArrayType>::get() ),
				 co::MissingServiceException );

	// getting the service for a IStructType should still return an IAttributeInfo
	co::Interface* itf = sm->getServiceForType( memberInfoType, co::typeOf<co::IStructType>::get() );
	EXPECT_EQ( "co.IAttributeInfo", itf->getInterfaceType()->getFullName() );

	// remove all custom service instances
	sm->removeServiceForType( memberInfoType, co::typeOf<co::IAttributeContainer>::get() );
	sm->removeServiceForType( memberInfoType, co::typeOf<co::IInterfaceType>::get() );

	// the whole service record should have been removed
	try
	{
		sm->getService( memberInfoType );
		FAIL();
	}
	catch( co::MissingServiceException& e )
	{
		EXPECT_EQ( "unknown service", e.getMessage() );
	}
}
