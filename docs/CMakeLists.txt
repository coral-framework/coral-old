################################################################################
# Doxygen
################################################################################

PROJECT( DOCS )

SET( CORAL_PATH
	"${CMAKE_SOURCE_DIR}/modules"
	"${CMAKE_BINARY_DIR}/modules"
)

CORAL_GENERATE_DOX( co "${CMAKE_CURRENT_BINARY_DIR}/co" )
CORAL_GENERATE_DOXYGEN( docs Doxyfile.in DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/co/__module.dox" )
