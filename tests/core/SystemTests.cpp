/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/System.h>
#include <co/TypeManager.h>
#include <gtest/gtest.h>

TEST( SystemTests, coralPathDirsAreUnique )
{
	co::ArrayRange<const std::string> pathsA = co::getPaths();
	ASSERT_GT( pathsA.getSize(), static_cast<size_t>( 0 ) );

	// adding the same dirs again shouldn't change the CORAL_PATH
	std::string morePaths;
	for( co::ArrayRange<const std::string> it( pathsA ); it; it.popFirst() )
	{
		morePaths.append( it.getFirst() );
		morePaths.append( ";" );
	}
	co::addPath( morePaths );

	co::ArrayRange<const std::string> pathsB = co::getPaths();
	EXPECT_EQ( pathsA.getSize(), pathsB.getSize() );
}

TEST( SystemTests, startupShutdown )
{
	co::RefPtr<co::System> system1 = co::getSystem();
	ASSERT_TRUE( system1.isValid() );

	co::shutdown();

	co::RefPtr<co::System> system2 = co::getSystem();
	ASSERT_TRUE( system2.isValid() );

	ASSERT_NE( system1, system2 );

	// system1 will be destroyed at the end of this test, system2 will remain
}

TEST( SystemTests, typeManager )
{
	co::RefPtr<co::System> system = co::getSystem();
	co::RefPtr<co::TypeManager> types = system->getTypes();
	ASSERT_TRUE( types.isValid() );
	EXPECT_TRUE( types->findType( "co.System" ) != NULL );
}
