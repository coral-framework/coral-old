################################################################################
# About This Directory:
#	This is the parent dir of all test projects.
################################################################################

# Locate the Google C++ Testing Framework.

FIND_PACKAGE( GTest )

# We can only add the test projects if GTest is available.
IF( GTEST_FOUND )

	ADD_SUBDIRECTORY( core )
	ADD_SUBDIRECTORY( moduleA )
	ADD_SUBDIRECTORY( moduleB )
	ADD_SUBDIRECTORY( regen )

	# Add a custom target to build all tests
	ADD_CUSTOM_TARGET( tests DEPENDS tests_core tests_moduleA tests_moduleB tests_regen )

ELSE()

	MESSAGE( "WARNING: GTest was NOT FOUND, all automated testing projects will be disabled." )

ENDIF()
