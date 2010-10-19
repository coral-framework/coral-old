/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/Coral.h>
#include <co/System.h>
#include <co/ArrayType.h>
#include <co/Component.h>
#include <co/MemberInfo.h>
#include <co/StructType.h>
#include <co/TypeManager.h>
#include <co/InterfaceType.h>
#include <co/ServiceManager.h>
#include <co/AttributeContainer.h>
#include <co/TypeCreationTransaction.h>
#include <co/MissingServiceException.h>
#include <co/IllegalArgumentException.h>
#include <co/NoSuchInterfaceException.h>
#include <gtest/gtest.h>

TEST( ServiceManagerTests, globalService )
{
	co::ServiceManager* sm = co::getSystem()->getServices();

	// try to get the TypeManager service before it's been added
	co::InterfaceType* typeManagerType = co::typeOf<co::TypeManager>::get();
	EXPECT_THROW( sm->getService( NULL ), co::IllegalArgumentException );
	EXPECT_THROW( sm->getService( typeManagerType ), co::MissingServiceException );

	// add the TypeManager service
	EXPECT_THROW( sm->addService( typeManagerType, NULL ), co::IllegalArgumentException );
	EXPECT_THROW( sm->addService( typeManagerType, co::getSystem() ), co::IllegalArgumentException );
	sm->addService( typeManagerType, co::getSystem()->getTypes() );

	// now we should be able to get it
	EXPECT_TRUE( sm->getService( typeManagerType ) != NULL );

	// test the utility functions
	EXPECT_EQ( sm->getService( typeManagerType ), co::getServiceForType( typeManagerType, NULL ) );
	EXPECT_EQ( sm->getService( typeManagerType ), co::getService<co::TypeManager>() );

	// remove the service & check if it's been removed
	sm->removeService( typeManagerType );
	EXPECT_THROW( sm->removeService( typeManagerType ), co::MissingServiceException );
	EXPECT_THROW( sm->getService( typeManagerType ), co::MissingServiceException );
}

TEST( ServiceManagerTests, lazyGlobalService )
{
	co::ServiceManager* sm = co::getSystem()->getServices();

	// make sure the service has not been added yet
	co::InterfaceType* tctType = co::typeOf<co::TypeCreationTransaction>::get();
	EXPECT_THROW( sm->getService( tctType ), co::MissingServiceException );

	// add the service with lazy instantiation
	EXPECT_TRUE( sm->getIsLazy() );
	EXPECT_THROW( sm->addServiceImplementation( NULL, "co.TypeCreationTransactionComponent" ),
					co::IllegalArgumentException );
	EXPECT_THROW( sm->addServiceImplementation( tctType, "invalid" ), co::IllegalArgumentException );
	EXPECT_THROW( sm->addServiceImplementation( co::typeOf<co::Type>::get(),
					"co.TypeCreationTransactionComponent" ), co::NoSuchInterfaceException );
	sm->addServiceImplementation( tctType, "co.TypeCreationTransactionComponent" );

	// now we should be able to get the service instance
	co::Interface* service = sm->getService( tctType );
	EXPECT_TRUE( service != NULL );
	EXPECT_EQ( tctType, service->getInterfaceType() );

	// remove the service
	co::TypeCreationTransaction* tct = dynamic_cast<co::TypeCreationTransaction*>( service );
	ASSERT_TRUE( tct != NULL );

	tct->rollback();
	sm->removeService( tctType );

	EXPECT_THROW( sm->getService( tctType ), co::MissingServiceException );
}

TEST( ServiceManagerTests, customServicesPerType )
{
	co::ServiceManager* sm = co::getSystem()->getServices();

	// make sure the service has not been added yet
	co::InterfaceType* memberInfoType = co::typeOf<co::MemberInfo>::get();
	EXPECT_THROW( sm->getService( memberInfoType ), co::MissingServiceException );

	// add a custom instance for AttributeContainers
	co::InterfaceType* attribContType = co::typeOf<co::AttributeContainer>::get();
	co::Component* component = co::newInstance( "co.AttributeInfoComponent" );
	sm->addServiceForType( memberInfoType, attribContType, component->getFacet<co::MemberInfo>() );

	// add a different custom instance for InterfaceTypes
	co::InterfaceType* interfaceTypeType = co::typeOf<co::InterfaceType>::get();
	sm->addServiceImplementationForType( memberInfoType, interfaceTypeType, "co.InterfaceInfoComponent" );

	// getting the service for an ArrayType should hit an unavailable global instance
	EXPECT_THROW( sm->getServiceForType( memberInfoType, co::typeOf<co::ArrayType>::get() ),
					co::MissingServiceException );

	// getting the service for a StructType should return an AttributeInfo
	co::Interface* itf = sm->getServiceForType( memberInfoType, co::typeOf<co::StructType>::get() );
	EXPECT_EQ( "co.AttributeInfo", itf->getInterfaceType()->getFullName() );

	// getting the service for an InterfaceType should return an InterfaceInfo
	itf = sm->getServiceForType( memberInfoType, interfaceTypeType );
	EXPECT_EQ( "co.InterfaceInfo", itf->getInterfaceType()->getFullName() );

	// test the utility functions
	EXPECT_EQ( itf, co::getServiceForType( memberInfoType, interfaceTypeType ) );
	EXPECT_EQ( itf, co::getService<co::MemberInfo>( interfaceTypeType ) );

	// add a global instance for the service
	sm->addServiceImplementation( memberInfoType, "co.MethodInfoComponent" );

	// now, getting the service for an ArrayType should return a MethodInfo
	co::InterfaceType* arrayTypeType = co::typeOf<co::ArrayType>::get();
	itf = sm->getServiceForType( memberInfoType, arrayTypeType );
	EXPECT_EQ( "co.MethodInfo", itf->getInterfaceType()->getFullName() );

	// test the utility functions
	EXPECT_EQ( itf, co::getServiceForType( memberInfoType, arrayTypeType ) );
	EXPECT_EQ( itf, co::getService<co::MemberInfo>( arrayTypeType ) );

	// getting the service for a StructType should still return an AttributeInfo
	itf = sm->getServiceForType( memberInfoType, co::typeOf<co::StructType>::get() );
	EXPECT_EQ( "co.AttributeInfo", itf->getInterfaceType()->getFullName() );

	// getting the service for an InterfaceType should still return an InterfaceInfo
	itf = sm->getServiceForType( memberInfoType, interfaceTypeType );
	EXPECT_EQ( "co.InterfaceInfo", itf->getInterfaceType()->getFullName() );
}

TEST( ServiceManagerTests, customServicesPerInstance )
{
	// --- assumes the services added in 'customServicesPerType' have not been removed ---
	co::ServiceManager* sm = co::getSystem()->getServices();
	co::InterfaceType* memberInfoType = co::typeOf<co::MemberInfo>::get();

	// getting a MemberInfo service for a MethodInfo instance should return the intance's interface
	co::MemberInfo* aMethod = co::typeOf<co::Interface>::get()->getMember( "componentRetain" );
	ASSERT_TRUE( aMethod != NULL );

	co::Interface* itf = sm->getServiceForInstance( memberInfoType, aMethod );
	co::MemberInfo* mi = dynamic_cast<co::MemberInfo*>( itf );
	ASSERT_TRUE( mi != NULL );
	EXPECT_EQ( "componentRetain", mi->getName() );

	EXPECT_EQ( itf, co::getServiceForInstance( memberInfoType, aMethod ) );
	EXPECT_EQ( itf, co::getService<co::MemberInfo>( aMethod ) );

	// getting a MemberInfo service for the system should return the global service instance
	itf = sm->getServiceForInstance( memberInfoType, co::getSystem() );
	EXPECT_EQ( "co.MethodInfo", itf->getInterfaceType()->getFullName() );

	EXPECT_EQ( itf, co::getServiceForInstance( memberInfoType, co::getSystem() ) );
	EXPECT_EQ( itf, co::getService<co::MemberInfo>( co::getSystem() ) );

	/*
		Getting a MemberInfo service for an InterfaceType instance should return
			the InterfaceType-specialized service instance.
	 */
	co::Interface* clientInstance = co::disambiguate<co::Interface>( co::typeOf<co::Type>::get() );
	itf = sm->getServiceForInstance( memberInfoType, clientInstance );
	EXPECT_EQ( "co.InterfaceInfo", itf->getInterfaceType()->getFullName() );

	EXPECT_EQ( itf, co::getServiceForInstance( memberInfoType, clientInstance ) );
	EXPECT_EQ( itf, co::getService<co::MemberInfo>( clientInstance ) );
}

TEST( ServiceManagerTests, serviceRemovals )
{
	// --- assumes the services added in 'customServicesPerType' have not been removed ---
	co::ServiceManager* sm = co::getSystem()->getServices();
	co::InterfaceType* memberInfoType = co::typeOf<co::MemberInfo>::get();

	// remove the service's global instance
	sm->removeService( memberInfoType );

	// getting the service for an ArrayType should hit an unavailable global instance
	EXPECT_THROW( sm->getServiceForType( memberInfoType, co::typeOf<co::ArrayType>::get() ),
				 co::MissingServiceException );

	// getting the service for a StructType should still return an AttributeInfo
	co::Interface* itf = sm->getServiceForType( memberInfoType, co::typeOf<co::StructType>::get() );
	EXPECT_EQ( "co.AttributeInfo", itf->getInterfaceType()->getFullName() );

	// remove all custom service instances
	sm->removeServiceForType( memberInfoType, co::typeOf<co::AttributeContainer>::get() );
	sm->removeServiceForType( memberInfoType, co::typeOf<co::InterfaceType>::get() );

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
