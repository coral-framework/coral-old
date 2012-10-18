################################################################################
# Documentation Generation
################################################################################

project( DOCS )

find_program( NANOC "nanoc" )

if( NANOC )

	# Generate the General Documentation

	configure_file( config.yaml.in "${CMAKE_CURRENT_SOURCE_DIR}/config.yaml" )

	add_custom_target( docs-general
		COMMAND ${NANOC} compile
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Running nanoc to compile the general documentation..."
	)

	# Generate the API Documentation

	set( CORAL_PATH
		"${CMAKE_SOURCE_DIR}/modules"
		"${CMAKE_BINARY_DIR}/modules"
	)

	CORAL_GENERATE_DOX( docs-csl2dox co "${CMAKE_CURRENT_BINARY_DIR}/co" )

	CORAL_GENERATE_DOXYGEN( docs "doxygen/Doxyfile.in" )
	if( DOXYGEN_FOUND )
		add_dependencies( docs docs-general docs-csl2dox )
	endif()

else()

	message( "WARNING: nanoc not found - documentation generation has been disabled." )

endif()
