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
MACRO( CORAL_ADD_TO_MAKE_CLEAN filename )
	SET_PROPERTY( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${filename}" )
ENDMACRO( CORAL_ADD_TO_MAKE_CLEAN )

################################################################################
# Doxygen Macro
################################################################################
# Creates an optional target to generate documentation using Doxygen.
#
# This macro will preprocess the file "${CMAKE_CURRENT_SOURCE_DIR}/${doxyfileName}"
# using CONFIGURE_FILE(), save the result to ${CMAKE_CURRENT_BINARY_DIR} and then
# run Doxygen on the file from that dir.
#
# Parameters:
#     targetName     Name given to the custom target.
#     doxyfileName   Name of the Doxygen config file in the current source dir.
#
# Extra arguments are passed along to Doxygen when invoking the command.
#
MACRO( CORAL_GENERATE_DOXYGEN targetName doxyfileName )

	FIND_PACKAGE( Doxygen )

	IF( DOXYGEN_FOUND )
		SET( DOXYFILE_FOUND false )
		IF( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${doxyfileName}" )
			SET( DOXYFILE_FOUND true )
		ENDIF()

		IF( DOXYFILE_FOUND )

			MESSAGE( STATUS "Setting up Doxygen target '${targetName}'..." )

			CONFIGURE_FILE( "${CMAKE_CURRENT_SOURCE_DIR}/${doxyfileName}" "${CMAKE_CURRENT_BINARY_DIR}/${doxyfileName}.configured" )

			ADD_CUSTOM_TARGET( ${targetName}
				COMMENT "Running Doxygen..."
				WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
				COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/${doxyfileName}.configured" ${ARGN}
			)

			SET_TARGET_PROPERTIES( ${targetName} PROPERTIES PROJECT_LABEL "Doxygen" )

			# Add .tag file and generated documentation to the list of files we must erase when distcleaning

			# Read doxygen configuration file
			FILE( READ "${CMAKE_CURRENT_BINARY_DIR}/${doxyfileName}.configured" DOXYFILE_CONTENTS )
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
				CORAL_ADD_TO_MAKE_CLEAN( "${CMAKE_CURRENT_BINARY_DIR}/${DOXYGEN_TAG_FILE}" )
			ENDIF()

			IF( DOXYGEN_OUTPUT_DIRECTORY AND DOXYGEN_HTML_OUTPUT )
				CORAL_ADD_TO_MAKE_CLEAN( "${DOXYGEN_OUTPUT_DIRECTORY}/${DOXYGEN_HTML_OUTPUT}" )
			ENDIF()

		ELSE( DOXYFILE_FOUND )
			MESSAGE( SEND_ERROR "Doxyfile not found - configuration error." )
		ENDIF( DOXYFILE_FOUND )
	ELSE( DOXYGEN_FOUND )
		MESSAGE( "Doxygen not found - documentation will not be generated." )
	ENDIF( DOXYGEN_FOUND )

ENDMACRO( CORAL_GENERATE_DOXYGEN )
