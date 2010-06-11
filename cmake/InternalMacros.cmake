################################################################################
# Common Global Initialization
################################################################################
IF( APPLE )
	# On OSX 10.6 (Darwin 10.0), use only the x86_64 architecture by default
	IF( NOT ${CMAKE_HOST_SYSTEM_VERSION} VERSION_LESS 10 )
		SET( CMAKE_OSX_ARCHITECTURES "x86_64" )
		SET( CMAKE_OSX_DEPLOYMENT_TARGET "10.6" )
	ENDIF()
ENDIF()

################################################################################
# Macro to set common properties for a default target (executables/libs)
################################################################################
MACRO( CORAL_DEFAULT_TARGET_PROPERTIES targetName )

	# On Windows, artifacts get a 'D' suffix when built in Debug mode
	IF( WIN32 )
		SET_PROPERTY( TARGET ${targetName} PROPERTY OUTPUT_NAME_DEBUG "${targetName}_debug" )

		# Prevent the MSVC IDE from creating targets in "Debug"/"Release" subdirs
		IF( MSVC_IDE )
			SET_TARGET_PROPERTIES( ${targetName} PROPERTIES PREFIX "../" )
		ENDIF()

		# Generate all executables and shared libs (but not module libs) in /bin
		GET_TARGET_PROPERTY( _targetType ${targetName} TYPE )
		IF( NOT _targetType STREQUAL "MODULE_LIBRARY" )
			SET_TARGET_PROPERTIES( ${targetName} PROPERTIES
				RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
				LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
			)
		ENDIF()
	ENDIF()

	IF( MSVC )
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
			# On Snow Leopard, force usage of the x86_64 arch.
			SET_TARGET_PROPERTIES( ${targetName} PROPERTIES
				XCODE_ATTRIBUTE_ARCHS ${CMAKE_OSX_ARCHITECTURES}
			)
		ENDIF()
	ENDIF()

ENDMACRO( CORAL_DEFAULT_TARGET_PROPERTIES )

################################################################################
# Macro to set common properties for a module target (Coral Module)
################################################################################
MACRO( CORAL_MODULE_TARGET_PROPERTIES moduleName )

	CORAL_DEFAULT_TARGET_PROPERTIES( ${moduleName} )

	# Add a suffix to module libraries built in Debug mode
	SET_PROPERTY( TARGET ${moduleName} PROPERTY LIBRARY_OUTPUT_NAME_DEBUG "${moduleName}_debug" )

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
# Macro to set common properties for a test target (test executable)
################################################################################
MACRO( CORAL_TEST_TARGET_PROPERTIES targetName testExecutableVar )

	CORAL_DEFAULT_TARGET_PROPERTIES( ${targetName} )

	IF( XCODE_VERSION )
		GET_TARGET_PROPERTY( _location ${targetName} LOCATION )
		STRING( REPLACE "/$(CONFIGURATION)/" "/\$ENV{CONFIGURATION}/" ${testExecutableVar} ${_location} )
	ELSE()
		IF( CMAKE_BUILD_TYPE MATCHES "Debug" )
			GET_TARGET_PROPERTY( ${testExecutableVar} ${targetName} DEBUG_LOCATION )
		ELSE()
			GET_TARGET_PROPERTY( ${testExecutableVar} ${targetName} LOCATION )
		ENDIF()
	ENDIF()

ENDMACRO( CORAL_TEST_TARGET_PROPERTIES )

################################################################################
# Macro to enable generation of 'test coverage' data for a target.
# Only works on UNIX, and only if the global var 'TEST_COVERAGE' is enabled.
################################################################################
MACRO( CORAL_ENABLE_TEST_COVERAGE targetName )
	IF( UNIX AND TEST_COVERAGE )
		GET_TARGET_PROPERTY( _COMPILE_FLAGS ${targetName} COMPILE_FLAGS )
		IF( ${_COMPILE_FLAGS} MATCHES NOTFOUND )
			SET( _COMPILE_FLAGS "" )
		ENDIF( ${_COMPILE_FLAGS} MATCHES NOTFOUND )

		GET_TARGET_PROPERTY( _LINK_FLAGS ${targetName} LINK_FLAGS )
		IF( ${_LINK_FLAGS} MATCHES NOTFOUND )
			SET( _LINK_FLAGS "" )
		ENDIF( ${_LINK_FLAGS} MATCHES NOTFOUND )

		SET_TARGET_PROPERTIES( ${targetName} PROPERTIES
			COMPILE_FLAGS "${_COMPILE_FLAGS} -fprofile-arcs -ftest-coverage"
			LINK_FLAGS    "${_LINK_FLAGS} -fprofile-arcs -ftest-coverage"
		)
	ENDIF()
ENDMACRO( CORAL_ENABLE_TEST_COVERAGE )

################################################################################
# Adds a file to the list of files to be cleaned in a directory
################################################################################
MACRO( CORAL_ADD_TO_CLEAN filename )
	SET_PROPERTY( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${filename}" )
ENDMACRO( CORAL_ADD_TO_CLEAN )

################################################################################
# Doxygen Macro
################################################################################
# Creates an optional target to generate documentation using Doxygen.
#
# This command will pre-process the file "${CMAKE_CURRENT_SOURCE_DIR}/${doxyfile_name}",
# using the CONFIGURE_FILE() command, save it to "${CMAKE_CURRENT_BINARY_DIR}/${doxyfile_name}"
# and then run doxygen on the saved file within the ${CMAKE_CURRENT_BINARY_DIR} dir.
#
# Prototype:
#     CORAL_GENERATE_DOXYGEN( target_name doxyfile_name )
# Parameters:
#     target_name     Name given to the target that generates the documentation.
#     doxyfile_name   Name of the Doxygen config file.
#
MACRO( CORAL_GENERATE_DOXYGEN target_name doxyfile_name )
	FIND_PACKAGE( Doxygen )
	IF( DOXYGEN_FOUND )
		SET( DOXYFILE_FOUND false )
		IF( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${doxyfile_name}" )
			SET( DOXYFILE_FOUND true )
		ENDIF()

		IF( DOXYFILE_FOUND )

			MESSAGE( STATUS "Setting up Doxygen target '${target_name}'..." )

			CONFIGURE_FILE( "${CMAKE_CURRENT_SOURCE_DIR}/${doxyfile_name}" "${CMAKE_CURRENT_BINARY_DIR}/${doxyfile_name}.configured" )

			ADD_CUSTOM_TARGET( ${target_name} ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/${doxyfile_name}.configured" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" )

			SET_TARGET_PROPERTIES( ${target_name} PROPERTIES PROJECT_LABEL "Doxygen" )

			# Add .tag file and generated documentation to the list of files we must erase when distcleaning

			# Read doxygen configuration file
			FILE( READ "${CMAKE_CURRENT_BINARY_DIR}/${doxyfile_name}.configured" DOXYFILE_CONTENTS )
			STRING( REGEX REPLACE "\n" ";" DOXYFILE_LINES ${DOXYFILE_CONTENTS} )

			# Parse .tag filename and add to list of files to delete if it exists
			SET( DOXYGEN_TAG_FILE )
			SET( DOXYGEN_OUTPUT_DIRECTORY )
			SET( DOXYGEN_HTML_OUTPUT )

			FOREACH( DOXYLINE ${DOXYFILE_LINES} )
				STRING( REGEX REPLACE ".*GENERATE_TAGFILE *= *([^^\n]+).*" "\\1" DOXYGEN_TAG_FILE			"${DOXYLINE}" )
				STRING( REGEX REPLACE ".*OUTPUT_DIRECTORY *= *([^^\n]+).*" "\\1" DOXYGEN_OUTPUT_DIRECTORY	"${DOXYLINE}" )
				STRING( REGEX REPLACE ".*HTML_OUTPUT *= *([^^\n]+).*" "\\1"		 DOXYGEN_HTML_OUTPUT		"${DOXYLINE}" )
			ENDFOREACH( DOXYLINE )

			FILE( TO_CMAKE_PATH "${DOXYGEN_OUTPUT_DIRECTORY}" DOXYGEN_OUTPUT_DIRECTORY )
			FILE( TO_CMAKE_PATH "${DOXYGEN_HTML_OUTPUT}" DOXYGEN_HTML_OUTPUT )

			IF( DOXYGEN_TAG_FILE )
				CORAL_ADD_TO_CLEAN( "${CMAKE_CURRENT_BINARY_DIR}/${DOXYGEN_TAG_FILE}" )
			ENDIF()

			IF( DOXYGEN_OUTPUT_DIRECTORY AND DOXYGEN_HTML_OUTPUT )
				CORAL_ADD_TO_CLEAN( "${DOXYGEN_OUTPUT_DIRECTORY}/${DOXYGEN_HTML_OUTPUT}" )
			ENDIF()

		ELSE( DOXYFILE_FOUND )
			MESSAGE( SEND_ERROR "Doxyfile not found - Documentation will not be generated." )
		ENDIF( DOXYFILE_FOUND )
	ELSE( DOXYGEN_FOUND )
		MESSAGE( WARNING "Doxygen not found - Documentation will not be generated." )
	ENDIF( DOXYGEN_FOUND )
ENDMACRO( CORAL_GENERATE_DOXYGEN )
