#	Find the Coral framework.
#
#	This module defines the following variables:
#		CORAL_FOUND - Whether the Coral framework was found.
#		CORAL_INCLUDE_DIRS - Directories for including the Coral headers.
#		CORAL_LIBRARIES - Coral's optimized and debug libraries (whichever are available).
#		CORAL_LAUNCHER - Path to the 'coral' executable.
#
#	The following variables are accepted as input:
#		CORAL_ROOT (as a CMake or environment variable)
#			- Directory where Coral was installed (the "install prefix").
#		CORAL_PATH (as a CMake or environment variable)
#			- List of extra Coral repositories that should be passed to the Coral Compiler.
#
#	The following utility functions simplify the task of invoking the Coral Compiler.
#
#	function CORAL_GENERATE_MODULE( generatedSourceFiles moduleName [extraDependency ...] )
#		Generates source code, plus all required mappings, to implement the Coral module specified
#		by moduleName. All files are generated in a dir named "generated", relative to the current
#		CMake binary dir. Generated source files that should be added to the module's project are
#		added to the list variable indicated as the first parameter.
#
#	function CORAL_GENERATE_MAPPINGS( generatedHeaderFiles [typeName ...] )
#		Generates mappings for the given list of types. All files are generated in a dir named
#		"generated", relative to the current CMake binary dir. Generated files are added to the
#		list variable indicated as the first parameter.
#

################################################################################
# Initialization
################################################################################
if( NOT CORAL_PATH )
	if( ENV{CORAL_PATH} )
		set( CORAL_PATH $ENV{CORAL_PATH} )
	elseif( CORAL_ROOT )
		set( CORAL_PATH "${CORAL_ROOT}/modules" )
	elseif( ENV{CORAL_ROOT} )
		set( CORAL_PATH "$ENV{CORAL_ROOT}/modules" )
	endif()
endif()

if( APPLE )
	# On OSX use only the standard 64-bit architecture by default
	set( CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_64_BIT)" )
endif()

# Require minimal support for C++11 (static_assert, TR1, etc)
if( MSVC )
	if( MSVC_VERSION LESS 1600 )
		message( FATAL_ERROR "Coral currently requires Visual Studio 2010 or greater." )
	endif()
else()
	# Requires GCC 4.3+
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )
endif()

# Launcher arguments to invoke the coral compiler
set( CORAL_COMPILER_ARGS --csl acd lua.Launcher co.compiler.cli --ignoredupes )

################################################################################
# Function to get the current CORAL_PATH as a comma-separated string
################################################################################
function( CORAL_GET_PATH_STRING coralPathStr )
	set( result )
	foreach( repo ${CORAL_PATH} )
		if( result )
			set( result "${result},${repo}" )
		else()
			set( result "${repo}" )
		endif()
	endforeach()
	set( ${coralPathStr} ${result} PARENT_SCOPE )
endfunction()

################################################################################
# Function to generate mappings for a list of types
################################################################################
FUNCTION( CORAL_GENERATE_MAPPINGS generatedHeaders )
	SET( outDir "${CMAKE_CURRENT_BINARY_DIR}/generated" )

	# Initialize the list with important files that are always generated.
	SET( resultList
		"${outDir}/co/ISystem.h"
	)

	FOREACH( typeName ${ARGN} )
		STRING( REPLACE "." "/" typePath ${typeName} )
		LIST( APPEND resultList "${outDir}/${typePath}.h" )
	ENDFOREACH()

	LIST( REMOVE_DUPLICATES resultList )

	CORAL_GET_PATH_STRING( coralPathStr )

	ADD_CUSTOM_COMMAND( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/force_out_of_date"
		COMMAND ${CMAKE_COMMAND} -E echo "Forcing dependency check for Coral..."
	)

	ADD_CUSTOM_COMMAND( OUTPUT ${resultList}
		COMMAND ${CORAL_LAUNCHER} -p "${coralPathStr}" ${CORAL_LAUNCHER_FLAGS} ${CORAL_COMPILER_ARGS} ${ARGN}
		DEPENDS ${CORAL_LAUNCHER} "${CMAKE_CURRENT_BINARY_DIR}/force_out_of_date"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Running the Coral Compiler..."
	)

	SET( ${generatedHeaders} ${resultList} PARENT_SCOPE )

	# "make clean" should delete the coralc cache file
	SET_PROPERTY( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${outDir}/__coralc_cache.lua" )
ENDFUNCTION()

################################################################################
# Function to generate code for a module (all extra args are passed to coralc)
################################################################################
function( CORAL_GENERATE_MODULE generatedSourceFiles moduleName )
	set( outDir "${CMAKE_CURRENT_BINARY_DIR}/generated" )

	CORAL_GET_PATH_STRING( coralPathStr )

	# If the Coral Compiler is currently available...
	if( EXISTS ${CORAL_LAUNCHER} )
		# Use the Compiler to get the actual list of generated source files
		execute_process(
			COMMAND ${CORAL_LAUNCHER} -p "${coralPathStr}" ${CORAL_LAUNCHER_FLAGS}
						${CORAL_COMPILER_ARGS} --list -g ${moduleName} ${ARGN}
			OUTPUT_VARIABLE resultList
		)
		if( resultList MATCHES "\\*\\*\\* Error \\*\\*\\*" )
			string( SUBSTRING "${resultList}" 14 -1 errorMsg )
			message( FATAL_ERROR "Error reported by the Coral compiler: ${errorMsg}" )
		endif()
		string( REGEX REPLACE "([^\n]+)\n*" "${outDir}/\\1;" resultList ${resultList} )
	else()
		# Fall back to the default all-in-one source file
		set( resultList "${outDir}/__AllInOne.cpp" )
	endif()

	add_custom_command( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/force_out_of_date"
		COMMAND ${CMAKE_COMMAND} -E echo "Forcing dependency check for Coral..."
	)

	add_custom_command( OUTPUT ${resultList}
		COMMAND ${CORAL_LAUNCHER} -p "${coralPathStr}" ${CORAL_LAUNCHER_FLAGS}
					${CORAL_COMPILER_ARGS} -g ${moduleName} ${ARGN}
		DEPENDS ${CORAL_LAUNCHER} "${CMAKE_CURRENT_BINARY_DIR}/force_out_of_date"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Running the Coral Compiler..."
	)

	set( ${generatedSourceFiles} ${resultList} PARENT_SCOPE )

	# "make clean" should delete the coralc cache file
	set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${outDir}/__coralc_cache.lua" )
endfunction()

################################################################################
# Defines a target to generate docs for a module. Passes extra args to coralc.
################################################################################
FUNCTION( CORAL_GENERATE_DOX targetName moduleName outDir )
	CORAL_GET_PATH_STRING( coralPathStr )

	ADD_CUSTOM_TARGET( ${targetName}
		COMMAND ${CORAL_LAUNCHER} -p "${coralPathStr}" ${CORAL_LAUNCHER_FLAGS}
			${CORAL_COMPILER_ARGS} --dox -g ${moduleName} -o ${outDir} ${ARGN}
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Running the Coral Compiler to extract documentation..."
	)
ENDFUNCTION()

################################################################################
# Utility macro to set common build options for targets that use Coral.
################################################################################
MACRO( CORAL_TARGET targetName )

	# Artifacts always get a '_debug' suffix when built in debug mode
	SET_PROPERTY( TARGET ${targetName} PROPERTY DEBUG_POSTFIX "_debug" )

	# Targets built in 'RelWithDebInfo' mode are considered in 'release' mode
	SET_PROPERTY( TARGET ${targetName} APPEND PROPERTY COMPILE_DEFINITIONS_RELWITHDEBINFO "NDEBUG" )

	IF( WIN32 )
		# Generate executables in /bin and shared libs (but not module libs) in /lib
		GET_TARGET_PROPERTY( _targetType ${targetName} TYPE )
		IF( NOT _targetType STREQUAL "MODULE_LIBRARY" )
			SET_TARGET_PROPERTIES( ${targetName} PROPERTIES
				RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
				LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
				ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
			)
		ENDIF()
	ENDIF()

	# Disable unavoidable MSVC warnings
	IF( MSVC )
		ADD_DEFINITIONS( "/wd4251" ) # type needs to have dll-interface to be used by clients
		ADD_DEFINITIONS( "/wd4275" ) # non dll-interface class used as base for dll-interface class
		SET_PROPERTY( TARGET ${targetName} APPEND PROPERTY COMPILE_DEFINITIONS
				"_CRT_SECURE_NO_WARNINGS;_SCL_SECURE_NO_DEPRECATE" )
	ENDIF()

ENDMACRO( CORAL_TARGET )

################################################################################
# Utility macro to set common build options for Coral Module targets.
################################################################################
macro( CORAL_MODULE_TARGET moduleName targetName )
	CORAL_TARGET( ${targetName} )

	# Copy or generate the module library into /modules/${modulePath}/
	string( REPLACE "." "/" modulePath ${moduleName} )
	if( XCODE_VERSION OR MSVC_IDE )
		# Copy the library after linking (makes sense for IDE's that create intermediate dirs)
		file( MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/modules/${modulePath}" )
		if( XCODE_VERSION )
			set( targetFileName "$(FULL_PRODUCT_NAME)" )
		else()
			set( targetFileName "$(TargetFileName)" ) # MSVC_IDE
		endif()
		add_custom_command( TARGET ${targetName} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${targetFileName}" "${CMAKE_BINARY_DIR}/modules/${modulePath}/"
			COMMENT "Copying module '${moduleName}'..."
		)
	else()
		# Create the library directly in the output dir
		set_target_properties( ${targetName} PROPERTIES
			LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/${modulePath}"
		)
	endif()

endmacro( CORAL_MODULE_TARGET )

################################################################################
# Utility macro to set env vars for a test so it finds the coral library
################################################################################
MACRO( CORAL_TEST_ENVIRONMENT testName )

	IF( NOT CORAL_ROOT )
		SET( CORAL_ROOT $ENV{CORAL_ROOT} )
	ENDIF()

	SET_PROPERTY(
		TEST ${testName}
		APPEND PROPERTY ENVIRONMENT
			PATH=${CORAL_ROOT}/lib
			LD_LIBRARY_PATH=${CORAL_ROOT}/lib
			DYLD_LIBRARY_PATH=${CORAL_ROOT}/lib
	)

ENDMACRO( CORAL_TEST_ENVIRONMENT )

################################################################################
# Macro to build a Coral module that *only* contains CSL types
################################################################################
macro( CORAL_BUILD_CSL_MODULE moduleName )
	CORAL_GENERATE_MODULE( _GENERATED_SOURCES ${moduleName} )
	include_directories( ${CORAL_INCLUDE_DIRS} "${CMAKE_CURRENT_BINARY_DIR}/generated" )
	add_library( ${moduleName} MODULE EXCLUDE_FROM_ALL ${_GENERATED_SOURCES} )
	CORAL_MODULE_TARGET( ${moduleName} ${moduleName} )
	target_link_libraries( ${moduleName} ${CORAL_LIBRARIES} )
	source_group( "@Generated" FILES ${_GENERATED_SOURCES} )
endmacro( CORAL_BUILD_CSL_MODULE )

################################################################################
# Find the Coral Framework
################################################################################
function( _coral_find_program _name )
	find_program( ${_name}
		NAMES ${ARGN}
		HINTS ".."
		PATHS "${CORAL_ROOT}" ENV CORAL_ROOT
		PATH_SUFFIXES "." "bin"
	)
	mark_as_advanced( ${_name} )
endfunction()

function( _coral_find_library _name )
	find_library( ${_name}
		NAMES ${ARGN}
		HINTS ".."
		PATHS "${CORAL_ROOT}" ENV CORAL_ROOT
		PATH_SUFFIXES "lib"
	)
	mark_as_advanced( ${_name} )
endfunction()

find_path( CORAL_INCLUDE_DIRS "co/Coral.h"
	HINTS ".."
	PATHS "${CORAL_ROOT}" ENV CORAL_ROOT
	PATH_SUFFIXES "include"
)

if( NOT CORAL_LAUNCHER )
	_coral_find_program( CORAL_LAUNCHER "coral.exe" "coral" )
endif()

_coral_find_library( CORAL_LIBRARY "coral" )
_coral_find_library( CORAL_LIBRARY_DEBUG "coral_debug" )

if( CORAL_LIBRARY AND CORAL_LIBRARY_DEBUG )
	set( CORAL_LIBRARIES optimized ${CORAL_LIBRARY} debug ${CORAL_LIBRARY_DEBUG} )
elseif( CORAL_LIBRARY )
	set( CORAL_LIBRARIES optimized ${CORAL_LIBRARY} )
elseif( CORAL_LIBRARY_DEBUG )
	set( CORAL_LIBRARIES debug ${CORAL_LIBRARY_DEBUG} )
endif()

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Coral DEFAULT_MSG CORAL_INCLUDE_DIRS CORAL_LIBRARIES CORAL_LAUNCHER )
