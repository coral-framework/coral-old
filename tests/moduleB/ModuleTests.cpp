/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <gtest/gtest.h>

#include <co/Coral.h>
#include <co/IModule.h>
#include <co/ISystem.h>
#include <co/IObject.h>
#include <co/IReflector.h>
#include <co/ModuleState.h>
#include <co/ICompositeType.h>
#include <co/IField.h>
#include <co/IModuleManager.h>
#include <co/IServiceManager.h>
#include <co/LifeCycleException.h>
#include <co/ITypeTransaction.h>
#include <co/IllegalArgumentException.h>
#include <moduleA/TestInterface.h>

TEST( ModuleTests, setupSystemThenLoadModuleA )
{
	// shutdown and re-setup the system
	co::shutdown();
	co::ISystem* system = co::getSystem();
	system->setup();

	// moduleA should not have been loaded yet
	EXPECT_TRUE( system->getModules()->findModule( "moduleA" ) == NULL );

	// load moduleA: first call should load, second call should just retrieve the module
	co::IModule* moduleA = system->getModules()->load( "moduleA" );
	ASSERT_TRUE( moduleA != NULL );
	EXPECT_EQ( moduleA, system->getModules()->load( "moduleA" ) );

	// now moduleA should have been loaded
	EXPECT_EQ( moduleA, system->getModules()->findModule( "moduleA" ) );

	// and its types should have reflectors
	co::IType* type = co::getType( "moduleA.TestInterface" );
	EXPECT_TRUE( type->getReflector() != NULL );
}

TEST( ModuleTests, setupSystemRequiringModuleB )
{
	// shutdown and re-setup the system requiring moduleB
	co::shutdown();
	co::ISystem* system = co::getSystem();

	// moduleA should not have been loaded yet
	EXPECT_TRUE( system->getModules()->findModule( "moduleA" ) == NULL );

	// setup the system requiring moduleB
	std::string requiredModule( "moduleB" );
	system->setup( co::Range<std::string const>( &requiredModule, 1 ) );

	// now moduleA should have been loaded, as a dependency of moduleB
	ASSERT_TRUE( system->getModules()->findModule( "moduleA" ) != NULL );

	// and its types should have reflectors
	co::IType* type = co::getType( "moduleA.TestInterface" );
	EXPECT_TRUE( type->getReflector() != NULL );
}

TEST( ModuleTests, systemAndModuleLifeCycles )
{
	// fully restart the system
	co::shutdown();
	co::ISystem* system = co::getSystem();

	// cannot tearDown() before setting the system up
	EXPECT_THROW( system->tearDown(), co::LifeCycleException );

	// cannot setupPresentation() before setupBase()
	EXPECT_THROW( system->setupPresentation(), co::LifeCycleException );

	// setupBase() requiring moduleA
	std::string requiredModule( "moduleA" );
	system->setupBase( co::Range<std::string const>( &requiredModule, 1 ) );

	// moduleA should have been loaded, but not moduleB
	co::IModule* moduleA = system->getModules()->findModule( "moduleA" );
	co::IModule* moduleB = system->getModules()->findModule( "moduleB" );
	ASSERT_TRUE( moduleA != NULL );
	ASSERT_TRUE( moduleB == NULL );

	// moduleA's state should be ModuleState_Integrated
	EXPECT_EQ( co::ModuleState_Integrated, moduleA->getState() );

	// cannot tearDown() because the system was not fully set up yet (setupPresentation())
	EXPECT_THROW( system->tearDown(), co::LifeCycleException );

	// setupPresentation()
	system->setupPresentation();

	// moduleA's state should now be ModuleState_PresentationIntegrated
	EXPECT_EQ( co::ModuleState_PresentationIntegrated, moduleA->getState() );

	// moduleB should not have been loaded yet
	ASSERT_TRUE( system->getModules()->findModule( "moduleB" ) == NULL );

	// loading moduleB now should bring it directly to the ModuleState_PresentationIntegrated state
	moduleB = system->getModules()->load( "moduleB" );
	ASSERT_TRUE( moduleB != NULL );
	EXPECT_EQ( co::ModuleState_PresentationIntegrated, moduleB->getState() );

	// ok to tearDown() now
	system->tearDown();

	// all modules should now be in the ModuleState_Disposed state
	EXPECT_EQ( co::ModuleState_Disposed, moduleA->getState() );
	EXPECT_EQ( co::ModuleState_Disposed, moduleB->getState() );

	// cannot load additional modules after the system is torn down
	EXPECT_THROW( system->getModules()->load( "whatever" ), co::LifeCycleException );

	// we need another full restart
	co::shutdown();
	system = co::getSystem();

	// re-setup
	system->setup();

	// cannot setup() the system again before tearing it down
	EXPECT_THROW( system->setup(), co::LifeCycleException );
}

TEST( ModuleTests, crossModuleInheritance )
{
	// instantiate a component from 'moduleA' that implements an interface from 'co'
	co::RefPtr<co::IObject> component = co::newInstance( "moduleA.TestComponent" );

	// exercise dynamic_casts
	moduleA::TestInterface* ti = component->getFacet<moduleA::TestInterface>();
	EXPECT_EQ( component.get(), ti->getInterfaceOwner() );

	co::ITypeTransaction* tct = component->getFacet<co::ITypeTransaction>();
	EXPECT_EQ( component.get(), tct->getInterfaceOwner() );
}

TEST( ModuleTests, crossModuleReflection )
{
	// exercise the reflection API on one of moduleA's types
	co::IType* type = co::getType( "moduleA.TestStruct" );
	co::IReflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	const int size = reflector->getSize();
	std::vector<co::uint8> instanceMemory( size );
	void* instancePtr = &instanceMemory.front();
	co::Any instanceAny( type, co::Any::VarIsPointer, instancePtr );

	EXPECT_THROW( reflector->createValue( instancePtr, 1337 ), co::IllegalArgumentException );
	EXPECT_NO_THROW( reflector->createValue( instancePtr, size ) );

	// get an IField
	co::ICompositeType* ct = dynamic_cast<co::ICompositeType*>( type );
	assert( ct );

	co::IField* anInt8Attrib = dynamic_cast<co::IField*>( ct->getMember( "anInt8" ) );
	assert( anInt8Attrib );

	// exercise the reflection API
	co::Any a1, a2;

	reflector->getAttribute( instanceAny, anInt8Attrib, a1 );
	EXPECT_EQ( 0, a1.get<int>() );

	a2.set( 7 );
	reflector->setAttribute( instanceAny, anInt8Attrib, a2 );

	reflector->getAttribute( instanceAny, anInt8Attrib, a1 );
	EXPECT_EQ( 7, a1.get<int>() );

	// test exception catching
	co::int32 garbage = -1;
	instanceAny.set( garbage );
	try
	{
		reflector->getAttribute( instanceAny, anInt8Attrib, a1 );
		FAIL();
	}
	catch( co::IllegalArgumentException& e )
	{
		EXPECT_EQ( "expected a valid moduleA::TestStruct*, but got (co::int32)-1", e.getMessage() );
	}

	reflector->destroyValue( instancePtr );
}

TEST( ModuleTests, serviceDependencies )
{
	// shutdown and re-setup the system
	co::shutdown();
	co::ISystem* system = co::getSystem();
	system->setup();

	co::IServiceManager* sm = system->getServices();
	co::IInterface* serviceType = co::typeOf<moduleA::TestInterface>::get();

	// register a service provided by moduleA
	ASSERT_TRUE( sm->getIsLazy() );
	sm->addServiceImplementation( serviceType, "moduleA.TestComponent" );

	// moduleA should not have been loaded yet
	EXPECT_TRUE( system->getModules()->findModule( "moduleA" ) == NULL );

	// now getting the service should cause moduleA to be loaded
	EXPECT_TRUE( co::getService<moduleA::TestInterface>() != NULL );

	// now moduleA should have been loaded
	EXPECT_TRUE( system->getModules()->findModule( "moduleA" ) != NULL );

	// we need another full restart
	co::shutdown();
	system = co::getSystem();
	system->setup();

	sm = system->getServices();
	serviceType = co::typeOf<moduleA::TestInterface>::get();

	// moduleA should not have been loaded yet
	EXPECT_TRUE( system->getModules()->findModule( "moduleA" ) == NULL );

	// re-register the service provided by moduleA, forcing its immediate instantiation
	sm->setIsLazy( false );
	ASSERT_FALSE( sm->getIsLazy() );
	sm->addServiceImplementation( serviceType, "moduleA.TestComponent" );

	// moduleA should already have been loaded now
	EXPECT_TRUE( system->getModules()->findModule( "moduleA" ) != NULL );

	// revert the isLazy option to its default value
	sm->setIsLazy( true );
	ASSERT_TRUE( sm->getIsLazy() );
}
