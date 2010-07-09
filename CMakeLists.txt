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
	ADD_DEFINITIONS( "/wd4251 /wd4275" )
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

# On Windows, look for libraries in a default 'SDK' dir.
IF( WIN32 )
	FILE( TO_CMAKE_PATH "$ENV{VR_SDK}" VR_SDK_DIR )
	IF( NOT VR_SDK_DIR )
		SET( VR_SDK_DIR "C:/SDK" )
	ENDIF( NOT VR_SDK_DIR )

	SET( GLOG_ROOT "${VR_SDK_DIR}/glog" )
	SET( GTEST_ROOT "${VR_SDK_DIR}/gtest" )
	SET( GFLAGS_ROOT "${VR_SDK_DIR}/gflags" )
	SET( CTEMPLATE_ROOT "${VR_SDK_DIR}/ctemplate" )
ENDIF()

# Google's Logging library is required.
FIND_PACKAGE( GLog REQUIRED )

# Google's commandline Flags library is required.
FIND_PACKAGE( GFlags REQUIRED )

# Include our own package, so we can use the CORAL_GENERATE_*() macros.
SET( CORAL_ROOT ${CMAKE_SOURCE_DIR} )
SET( CORAL_COMPILER coralc )
FIND_PACKAGE( Coral QUIET )

# Shared include paths for all subprojects
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR}/include ${GLOG_INCLUDE_DIRS} ${GFLAGS_INCLUDE_DIRS} )

################################################################################
# Global Coral Settings
################################################################################

# The official Coral Version
SET( CORAL_VERSION_MAJOR	"0" )
SET( CORAL_VERSION_MINOR	"1" )
SET( CORAL_VERSION_PATCH	"0" )
SET( CORAL_VERSION "${CORAL_VERSION_MAJOR}.${CORAL_VERSION_MINOR}.${CORAL_VERSION_PATCH}" )

# Current code generation revision.
SET( CORAL_COMPILER_OUTPUT_REVISION 1 )

################################################################################
# Packaging
################################################################################
SET( CPACK_PACKAGE_NAME					"coral" )
SET( CPACK_PACKAGE_VERSION_MAJOR		"${CORAL_VERSION_MAJOR}" )
SET( CPACK_PACKAGE_VERSION_MINOR		"${CORAL_VERSION_MINOR}" )
SET( CPACK_PACKAGE_VERSION_PATCH		"${CORAL_VERSION_PATCH}" )
SET( CPACK_PACKAGE_DESCRIPTION_SUMMARY	"Coral is a lightweight C++ component framework designed for soft real-time systems." )

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
