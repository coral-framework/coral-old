################################################################################
# Unit Tests
################################################################################

# Disable sign-compare warnings (unavoidable when using GTest)
if( NOT MSVC )
	add_definitions( "-Wno-sign-compare" )
endif()

# We require the GTest Framework
FIND_PACKAGE( GTest )
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
	MESSAGE( "WARNING: gtest not found - most tests have been disabled!" )
ENDIF()

################################################################################
# Smoke test the deployed executables
################################################################################

set( CORAL_PATH_STR "${CMAKE_BINARY_DIR}/modules,${CMAKE_SOURCE_DIR}/modules" )

# Test the Launcher Back End
add_test( NAME launcher COMMAND launcher -p ${CORAL_PATH_STR} lua.Launcher noscript )
set_tests_properties( launcher PROPERTIES
	PASS_REGULAR_EXPRESSION "launch script 'noscript' could not be found in the path"
)

# Test the Launcher Front End
add_test( NAME coral COMMAND coral -p ${CORAL_PATH_STR} lua.Launcher noscript )
set_tests_properties( coral PROPERTIES
	PASS_REGULAR_EXPRESSION "launch script 'noscript' could not be found in the path"
)

# Test the Compiler Front End
add_test( NAME coralc COMMAND coralc -g nomodule )
set_tests_properties( coralc PROPERTIES
	ENVIRONMENT "CORAL_PATH=${CORAL_PATH_STR}"
	PASS_REGULAR_EXPRESSION "Generating code for module 'nomodule'"
)
