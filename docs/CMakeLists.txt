################################################################################
# Doxygen
################################################################################

PROJECT( DOCS )

FIND_PROGRAM( NANOC "nanoc" )

IF( NANOC )

	# Generate the General Documentation

	CONFIGURE_FILE( config.yaml.in ${CMAKE_CURRENT_SOURCE_DIR}/config.yaml )

	ADD_CUSTOM_TARGET( docs-general
		COMMAND ${NANOC} compile
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Running nanoc to compile the general documentation..."
	)


	# Generate the API Documentation

	SET( CORAL_PATH
		"${CMAKE_SOURCE_DIR}/modules"
		"${CMAKE_BINARY_DIR}/modules"
	)

	CORAL_GENERATE_DOX( docs-csl2dox co "${CMAKE_CURRENT_BINARY_DIR}/co" )

	CORAL_GENERATE_DOXYGEN( docs doxygen/Doxyfile.in )

	ADD_DEPENDENCIES( docs docs-general docs-csl2dox )

ELSE()

	MESSAGE( WARNING "nanoc not found - cannot generate documentation." )

ENDIF()
