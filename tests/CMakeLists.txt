################################################################################
# About This Directory:
#	This is the parent dir of all test projects.
################################################################################

# Locate the Google C++ Testing Framework.

FIND_PACKAGE( GTest )

# We can only add the test projects if GTest is available.
IF( GTEST_FOUND )

	# Check if Valgrind is available for the memory check tests
	FIND_PROGRAM( VALGRIND_COMMAND valgrind )
	IF( NOT VALGRIND_COMMAND )
		MESSAGE( "WARNING: Valgrind was NOT FOUND - all memory tests have been disabled!" )
	ENDIF()

	ADD_SUBDIRECTORY( core )
	ADD_SUBDIRECTORY( lua )
	ADD_SUBDIRECTORY( moduleA )
	ADD_SUBDIRECTORY( moduleB )
	ADD_SUBDIRECTORY( regen )

	# Add a custom target to build all tests
	ADD_CUSTOM_TARGET( tests DEPENDS tests_core tests_lua tests_moduleA tests_moduleB tests_regen )

ELSE()

	MESSAGE( "WARNING: GTest was NOT FOUND, all automated testing projects will be disabled." )

ENDIF()
