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
# Macro to set common properties for CTest tests
################################################################################
MACRO( CORAL_DEFAULT_TEST_PROPERTIES testName )
	# currently empty
ENDMACRO( CORAL_DEFAULT_TEST_PROPERTIES )

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
# Extra args are passed at the end of the custom target's list of args to Doxygen.
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

			ADD_CUSTOM_TARGET( ${target_name}
				COMMENT "Running Doxygen..."
				WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
				COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/${doxyfile_name}.configured" ${ARGN}
			)

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
