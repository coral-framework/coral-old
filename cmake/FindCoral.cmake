#	Locate the Coral framework.
#
#	This module defines the following variables:
#		CORAL_FOUND - Found the Coral framework.
#		CORAL_INCLUDE_DIRS - Include directories.
#		CORAL_LAUNCHER - Path to the 'coral' executable.
#
#	It also defines the library variable below, which contain
#	debug/optimized keywords when a debug library is found:
#		CORAL_LIBRARIES - libcoral.
#
#	The following variables are accepted as input:
#		CORAL_ROOT - (as a CMake or environment variable)
#			The root directory of the Coral install prefix.
#		CORAL_PATH - (as a CMake or environment variable)
#			Controls the type repositories path passed to the Coral Compiler
#			when it is invoked by one of the helper functions listed below.
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
# Function to get the current CORAL_PATH as a CMake variable (List)
################################################################################
FUNCTION( CORAL_GET_PATH coralPath )
	IF( CORAL_PATH )
		SET( ${coralPath} ${CORAL_PATH} PARENT_SCOPE )
	ELSEIF( ENV{CORAL_PATH} )
		SET( ${coralPath} $ENV{CORAL_PATH} PARENT_SCOPE )
	ELSEIF( CORAL_ROOT )
		SET( ${coralPath} "${CORAL_ROOT}/modules" PARENT_SCOPE )
	ELSE()
		SET( ${coralPath} "$ENV{CORAL_ROOT}/modules" PARENT_SCOPE )
	ENDIF()
ENDFUNCTION()

################################################################################
# Function to get the current CORAL_PATH as a comma-separated string
################################################################################
FUNCTION( CORAL_GET_PATH_STRING coralPathStr )
	CORAL_GET_PATH( coralPath )
	SET( result )
	FOREACH( repo ${coralPath} )
		IF( result )
			SET( result "${result},${repo}" )
		ELSE()
			SET( result "${repo}" )
		ENDIF()
	ENDFOREACH()
	SET( ${coralPathStr} ${result} PARENT_SCOPE )
ENDFUNCTION()

################################################################################
# Internal Macro to gather the list of types in a module
################################################################################
MACRO( CORAL_GATHER_MODULE_TYPES _moduleTypeNames _moduleName )
	CORAL_GET_PATH( _coralPath )
	SET( _resultList )
	FOREACH( _repo ${_coralPath} )
		FILE( GLOB _cslFiles "${_repo}/${_moduleName}/*.csl" )
		FOREACH( _file ${_cslFiles} )
			# Get the type name and add it to the _resultList
			GET_FILENAME_COMPONENT( _name ${_file} NAME_WE )
			LIST( APPEND _resultList ${_name} )

			# Save the type's filename for later use in CORAL_GENERATE_MODULE.
			# We issue a warning if the same type is gathered twice from two different files.
			GET_FILENAME_COMPONENT( _filename ${_file} ABSOLUTE )
			SET( _filenameKey "${_moduleName}_${_name}_FILENAME" )
			IF( DEFINED ${_filenameKey} )
				IF( NOT ${_filenameKey} STREQUAL "${_filename}" )
					MESSAGE( WARNING "Clashing CSL files found: '${${_filenameKey}}' clashes with '${_filename}'." )
				ENDIF()
			ELSE()
				SET( ${_filenameKey} "${_filename}" )
			ENDIF()
		ENDFOREACH()
	ENDFOREACH()
	SET( ${_moduleTypeNames} ${_resultList} )
ENDMACRO()

################################################################################
# Function to generate mappings for a list of types
################################################################################
FUNCTION( CORAL_GENERATE_MAPPINGS generatedHeaders )
	SET( outDir "${CMAKE_CURRENT_BINARY_DIR}/generated" )

	# Initialize the list with important files that are always generated.
	SET( resultList
		"${outDir}/co/System.h"
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
		COMMAND ${CORAL_LAUNCHER} -p "${coralPathStr}" lua.Launcher co.compiler.cli ${ARGN}
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
FUNCTION( CORAL_GENERATE_MODULE generatedSourceFiles moduleName )
	SET( outDir "${CMAKE_CURRENT_BINARY_DIR}/generated" )

	# Initialize the list with files that are always generated.
	SET( resultList
		"${outDir}/__Bootstrap.cpp"
		"${outDir}/ModuleInstaller.h"
		"${outDir}/ModuleInstaller.cpp"
	)

	# Gather a list of the module's types.
	CORAL_GATHER_MODULE_TYPES( moduleTypeNames ${moduleName} )

	SET( hasModulePart FALSE )
	FOREACH( typeName ${moduleTypeNames} )
		IF( typeName STREQUAL "${moduleName}" )
			SET( hasModulePart TRUE )
		ENDIF()

		# Determine if the type is a component (if so, add the extra files)
		FILE( STRINGS "${${moduleName}_${typeName}_FILENAME}" isComponent LIMIT_COUNT 1 REGEX "component[ \t]+${typeName}" )
		IF( isComponent )
			LIST( APPEND resultList "${outDir}/${typeName}_Base.h" "${outDir}/${typeName}_Base.cpp" )
		ENDIF()

		LIST( APPEND resultList "${outDir}/${typeName}_Reflector.cpp" )
	ENDFOREACH()

	# If a ModulePart type was not declared, add the default one
	IF( NOT hasModulePart )
		LIST( APPEND resultList "${outDir}/__ModulePart.cpp" )
	ENDIF()

	CORAL_GET_PATH_STRING( coralPathStr )

	ADD_CUSTOM_COMMAND( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/force_out_of_date"
		COMMAND ${CMAKE_COMMAND} -E echo "Forcing dependency check for Coral..."
	)

	ADD_CUSTOM_COMMAND( OUTPUT ${resultList}
		COMMAND ${CORAL_LAUNCHER} -p "${coralPathStr}" lua.Launcher co.compiler.cli -g ${moduleName} ${ARGN}
		DEPENDS ${CORAL_LAUNCHER} "${CMAKE_CURRENT_BINARY_DIR}/force_out_of_date"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Running the Coral Compiler..."
	)

	SET( ${generatedSourceFiles} ${resultList} PARENT_SCOPE )

	# "make clean" should delete the coralc cache file
	SET_PROPERTY( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${outDir}/__coralc_cache.lua" )
ENDFUNCTION()

################################################################################
# Defines a target to generate docs for a module. Passes extra args to coralc.
################################################################################
FUNCTION( CORAL_GENERATE_DOX targetName moduleName outDir )
	CORAL_GET_PATH_STRING( coralPathStr )

	ADD_CUSTOM_TARGET( ${targetName}
		COMMAND ${CORAL_LAUNCHER} -p "${coralPathStr}" lua.Launcher co.compiler.cli --dox -g ${moduleName} -o ${outDir} ${ARGN}
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Running the Coral Compiler to extract documentation..."
	)
ENDFUNCTION()

################################################################################
# Common settings for the utility macros
################################################################################
IF( APPLE )
	# On OSX 10.6 (Darwin 10.0), use only the x86_64 architecture by default
	IF( NOT ${CMAKE_HOST_SYSTEM_VERSION} VERSION_LESS 10 )
		SET( CMAKE_OSX_ARCHITECTURES "x86_64" )
		SET( CMAKE_OSX_DEPLOYMENT_TARGET "10.6" )
	ENDIF()
ENDIF()

################################################################################
# Utility macro to set common properties for all targets
################################################################################
MACRO( CORAL_DEFAULT_TARGET_PROPERTIES targetName )

	# On Windows, artifacts get a '_debug' suffix when built in Debug mode
	IF( WIN32 )
		SET_PROPERTY( TARGET ${targetName} PROPERTY DEBUG_POSTFIX "_debug" )

		# Prevent the MSVC IDE from creating targets in "Debug"/"Release" subdirs
		IF( MSVC_IDE )
			SET_TARGET_PROPERTIES( ${targetName} PROPERTIES PREFIX "../" )
		ENDIF()

		# Generate executables in /bin and shared libs (but not module libs) in /lib
		GET_TARGET_PROPERTY( _targetType ${targetName} TYPE )
		IF( NOT _targetType STREQUAL "MODULE_LIBRARY" )
			SET_TARGET_PROPERTIES( ${targetName} PROPERTIES
				ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
				LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
				RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
			)
		ENDIF()
	ENDIF()

	# Disable unavoidable MSVC warnings
	IF( MSVC )
		ADD_DEFINITIONS( "/wd4275" ) # non dll-interface class used as base for dll-interface class
		SET_PROPERTY( TARGET ${targetName} APPEND PROPERTY COMPILE_DEFINITIONS
				"_CRT_SECURE_NO_WARNINGS;_SCL_SECURE_NO_DEPRECATE" )
	ENDIF()

	IF( XCODE_VERSION )
		IF( ${CMAKE_HOST_SYSTEM_VERSION} VERSION_LESS 10 )
			# On Leopard, force usage of GCC 4.2
			SET_TARGET_PROPERTIES( ${targetName} PROPERTIES
				XCODE_ATTRIBUTE_GCC_VERSION "4.2"
			)
		ELSE()
			# From Snow Leopard on, force usage of the x86_64 arch.
			SET_TARGET_PROPERTIES( ${targetName} PROPERTIES
				XCODE_ATTRIBUTE_ARCHS ${CMAKE_OSX_ARCHITECTURES}
			)
		ENDIF()
	ENDIF()

ENDMACRO( CORAL_DEFAULT_TARGET_PROPERTIES )

################################################################################
# Utility macro to set common properties for a module target (Coral Module)
################################################################################
MACRO( CORAL_MODULE_TARGET_PROPERTIES moduleName )

	# Add a suffix to module libraries built in Debug mode
	SET_PROPERTY( TARGET ${moduleName} PROPERTY DEBUG_POSTFIX "_debug" )

	# Copy or generate the module library into /modules/${moduleName}/
	IF( XCODE_VERSION )
		# Copy the library after linking (makes sense for IDE's that create intermediate dirs)
		FILE( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/modules/${moduleName} )
		ADD_CUSTOM_COMMAND( TARGET ${moduleName} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "$(CONFIGURATION_BUILD_DIR)/$(FULL_PRODUCT_NAME)" ${CMAKE_BINARY_DIR}/modules/${moduleName}/
			COMMENT "Copying module '${moduleName}'..."
		)
	ELSE()
		# Create the library directly in the output dir
		SET_TARGET_PROPERTIES( ${moduleName} PROPERTIES
			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules/${moduleName}
		)
	ENDIF()

ENDMACRO( CORAL_MODULE_TARGET_PROPERTIES )

################################################################################
# Utility macro to build a Coral Module that contains *only* CSL types
################################################################################
MACRO( CORAL_BUILD_CSL_MODULE moduleName )

	CORAL_GENERATE_MODULE( _GENERATED_SOURCES ${moduleName} )

	INCLUDE_DIRECTORIES( ${CORAL_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/generated )

	ADD_LIBRARY( ${moduleName} MODULE ${_GENERATED_SOURCES} )

	CORAL_DEFAULT_TARGET_PROPERTIES( ${moduleName} )
	CORAL_MODULE_TARGET_PROPERTIES( ${moduleName} )

	TARGET_LINK_LIBRARIES( ${moduleName} ${CORAL_LIBRARIES} )

	SOURCE_GROUP( "@Generated" FILES ${_GENERATED_SOURCES} )

ENDMACRO( CORAL_BUILD_CSL_MODULE )

################################################################################
# CMake Package Configuration
################################################################################
FUNCTION( _coral_find_program _name )
	FIND_PROGRAM( ${_name}
		NAMES ${ARGN}
		HINTS ".." "$ENV{CORAL_ROOT}" "${CORAL_ROOT}"
		PATH_SUFFIXES "." "bin"
	)
	MARK_AS_ADVANCED( ${_name} )
ENDFUNCTION()

FUNCTION( _coral_find_library _name )
	FIND_LIBRARY( ${_name}
		NAMES ${ARGN}
		HINTS ".." "$ENV{CORAL_ROOT}" "${CORAL_ROOT}"
		PATH_SUFFIXES "lib"
	)
	MARK_AS_ADVANCED( ${_name} )
ENDFUNCTION()

FIND_PATH( CORAL_INCLUDE_DIR co/Coral.h
	HINTS
		"../include"
		"$ENV{CORAL_ROOT}/include"
		"${CORAL_ROOT}/include"
)
MARK_AS_ADVANCED( CORAL_INCLUDE_DIR )

IF( NOT CORAL_LAUNCHER )
	_coral_find_program( CORAL_LAUNCHER		coral.bat coral )
ENDIF()

_coral_find_library( CORAL_LIBRARY			coral )
_coral_find_library( CORAL_LIBRARY_DEBUG	coral_debug )

IF( CORAL_LIBRARY AND CORAL_LIBRARY_DEBUG )
	SET( CORAL_LIBRARIES optimized ${CORAL_LIBRARY} debug ${CORAL_LIBRARY_DEBUG} )
ELSEIF( CORAL_LIBRARY )
	SET( CORAL_LIBRARIES general ${CORAL_LIBRARY} )
ELSEIF( CORAL_LIBRARY_DEBUG )
	SET( CORAL_LIBRARIES debug ${CORAL_LIBRARY_DEBUG} )
ENDIF()

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Coral DEFAULT_MSG CORAL_LIBRARIES CORAL_LAUNCHER CORAL_INCLUDE_DIR )

IF( CORAL_FOUND )
	SET( CORAL_INCLUDE_DIRS ${CORAL_INCLUDE_DIR} )
ENDIF()
