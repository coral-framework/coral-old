################################################################################
# Doxygen
################################################################################

PROJECT( DOCS )

FIND_PROGRAM( NANOC "nanoc" )

IF( NANOC )

	SET( CORAL_PATH
		"${CMAKE_SOURCE_DIR}/modules"
		"${CMAKE_BINARY_DIR}/modules"
	)

	CORAL_GENERATE_DOX( co "${CMAKE_CURRENT_BINARY_DIR}/co" )

	ADD_CUSTOM_COMMAND( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/output/doc/api/header.html"
		COMMAND ${NANOC} compile
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Compiling the general documentation (you must have nanoc installed)"
	)

	CORAL_GENERATE_DOXYGEN( docs doxygen/Doxyfile.in
		DEPENDS
			"${CMAKE_CURRENT_BINARY_DIR}/co/__module.dox"
			"${CMAKE_CURRENT_BINARY_DIR}/output/doc/api/header.html"
	)

ELSE()
	MESSAGE( WARNING "nanoc not found - Documentation will not be generated." )
ENDIF()
