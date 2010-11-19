################################################################################
# Initialization
################################################################################

CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( CORAL )

# Add our own modules dir to the modules path
SET( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH} )

# Include our custom definitions and macros
INCLUDE( InternalMacros )

################################################################################
# Global Stuff
################################################################################

IF( MSVC )
	# Disable unavoidable MSVC warnings
	ADD_DEFINITIONS( "/wd4251" )
ENDIF()

IF( UNIX )
	# Enable extra GCC warnings for extra-nice code
	ADD_DEFINITIONS( -Wall -Wextra -Wunused -Wwrite-strings )

	# GCC 4.3 introduced some senseless, annoying unavoidable warnings.
	INCLUDE( CheckCCompilerFlag )
	CHECK_C_COMPILER_FLAG( -Wno-ignored-qualifiers usingGCC43OrAbove )
	IF( usingGCC43OrAbove )
		ADD_DEFINITIONS( -Wno-ignored-qualifiers )
	ENDIF()

	# For C++ sources only:
	SET_PROPERTY( GLOBAL APPEND PROPERTY CMAKE_CXX_FLAGS "-Woverloaded-virtual" )
ENDIF()

# Include our own package, so we can use the CORAL_GENERATE_*() macros.
SET( CORAL_ROOT ${CMAKE_SOURCE_DIR} )
SET( CORAL_LAUNCHER launcher )
FIND_PACKAGE( Coral QUIET )

# Shared include paths for all subprojects
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR}/include )

################################################################################
# Global Coral Settings
################################################################################

# The official Coral Version
SET( CORAL_VERSION_MAJOR	"0" )
SET( CORAL_VERSION_MINOR	"5" )
SET( CORAL_VERSION_PATCH	"1" )
SET( CORAL_VERSION "${CORAL_VERSION_MAJOR}.${CORAL_VERSION_MINOR}.${CORAL_VERSION_PATCH}" )

# Code generation rules revision.
SET( CORAL_COMPILER_OUTPUT_REVISION 1 )

################################################################################
# Installation
################################################################################

# enforce a safe default CMAKE_INSTALL_PREFIX
IF( NOT CMAKE_INSTALL_PREFIX MATCHES "coral" )
	SET( CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/coral" )
ENDIF()

################################################################################
# Packaging
################################################################################
SET( CPACK_PACKAGE_NAME					"coral" )
SET( CPACK_PACKAGE_VERSION_MAJOR		"${CORAL_VERSION_MAJOR}" )
SET( CPACK_PACKAGE_VERSION_MINOR		"${CORAL_VERSION_MINOR}" )
SET( CPACK_PACKAGE_VERSION_PATCH		"${CORAL_VERSION_PATCH}" )
SET( CPACK_PACKAGE_DESCRIPTION_SUMMARY	"A lightweight C++ component framework for soft real-time systems." )

IF( HUDSON_JOBNAME )
	STRING( REPLACE "/label=" "-" _CPACK_PACKAGE_FILE_NAME "${HUDSON_JOBNAME}_$ENV{BUILD_ID}" )
	SET( CPACK_PACKAGE_FILE_NAME ${_CPACK_PACKAGE_FILE_NAME} )
ENDIF()

IF( WIN32 )
	SET( CPACK_GENERATOR "ZIP" )
ELSE()
	SET( CPACK_GENERATOR "TBZ2" )
ENDIF()

INCLUDE( CPack )

################################################################################
# Subdirectories
################################################################################

ADD_SUBDIRECTORY( docs )
ADD_SUBDIRECTORY( src )

ENABLE_TESTING()
ADD_SUBDIRECTORY( tests )
