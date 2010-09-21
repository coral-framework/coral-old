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

# Gets the current CORAL_PATH value.
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

# Gets the current CORAL_PATH, but returns as a comma-separated string, instead of a CMake list.
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

	ADD_CUSTOM_COMMAND( OUTPUT ${resultList}
		COMMAND ${CORAL_LAUNCHER} -p "${coralPathStr}" lua.Launcher co.compiler.cli -g ${moduleName} ${ARGN}
		DEPENDS ${CORAL_LAUNCHER}
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Generating code for module '${moduleName}'..."
	)

	SET( ${generatedSourceFiles} ${resultList} PARENT_SCOPE )
ENDFUNCTION()

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

	ADD_CUSTOM_COMMAND( OUTPUT ${resultList}
		COMMAND ${CORAL_LAUNCHER} -p "${coralPathStr}" lua.Launcher co.compiler.cli ${ARGN}
		DEPENDS ${CORAL_LAUNCHER}
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Generating required mappings..."
	)

	SET( ${generatedHeaders} ${resultList} PARENT_SCOPE )
ENDFUNCTION()

FUNCTION( _coral_find_program _name)
	FIND_PROGRAM( ${_name}
		NAMES ${ARGN}
		HINTS
			../
			$ENV{CORAL_ROOT}
			${CORAL_ROOT}
		PATH_SUFFIXES
			.
			bin
			bin/Release
			bin/Debug
	)
ENDFUNCTION()

FUNCTION( _coral_find_library _name)
	FIND_LIBRARY( ${_name}
		NAMES ${ARGN}
		HINTS
			../
			$ENV{CORAL_ROOT}
			${CORAL_ROOT}
		PATH_SUFFIXES
			lib
			lib/Release
			lib/Debug
	)
	MARK_AS_ADVANCED( ${_name} )
ENDFUNCTION()

FIND_PATH( CORAL_INCLUDE_DIR co/Coral.h
	HINTS
		../include
		$ENV{CORAL_ROOT}/include
		${CORAL_ROOT}/include
)
MARK_AS_ADVANCED( CORAL_INCLUDE_DIR )

IF( NOT CORAL_LAUNCHER )
	_coral_find_program( CORAL_LAUNCHER		coral.bat coral )
ENDIF()

_coral_find_library( CORAL_LIBRARY			coral )
_coral_find_library( CORAL_LIBRARY_DEBUG	corald )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Coral DEFAULT_MSG CORAL_LIBRARY CORAL_LAUNCHER CORAL_INCLUDE_DIR )

IF( CORAL_FOUND )
	SET( CORAL_INCLUDE_DIRS ${CORAL_INCLUDE_DIR} )
	IF( CORAL_LIBRARY_DEBUG )
		SET( CORAL_LIBRARIES optimized ${CORAL_LIBRARY} debug ${CORAL_LIBRARY_DEBUG} )
	ELSE()
		SET( CORAL_LIBRARIES ${CORAL_LIBRARY} )
	ENDIF()
ENDIF()
