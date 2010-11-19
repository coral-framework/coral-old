################################################################################
# About This Directory:
#	This is the parent dir of all test projects.
################################################################################

# Locate the Google C++ Testing Framework.

FIND_PACKAGE( GTest )

# We can only add the test projects if GTest is available.
IF( GTEST_FOUND )

	# On UNIX, GTest generally depends on pthreads, but CMake does not
	# add pthreads to GTEST_LIBRARIES. So we do it...
	IF( UNIX )
		LIST( APPEND GTEST_LIBRARIES pthread )
	ENDIF()

	# Check if Valgrind is available for the memory check tests
	FIND_PROGRAM( VALGRIND_COMMAND valgrind )
	IF( NOT VALGRIND_COMMAND )
		MESSAGE( "WARNING: valgrind not found - all memory tests have been disabled!" )
	ENDIF()

	ADD_SUBDIRECTORY( core )
	ADD_SUBDIRECTORY( lua )
	ADD_SUBDIRECTORY( moduleA )
	ADD_SUBDIRECTORY( moduleB )
	ADD_SUBDIRECTORY( regen )

	# Add a custom target to build all tests
	ADD_CUSTOM_TARGET( tests DEPENDS tests_core tests_lua tests_moduleA tests_moduleB tests_regen )

ELSE()

	MESSAGE( "WARNING: gtest not found - all tests have been disabled!" )

ENDIF()
