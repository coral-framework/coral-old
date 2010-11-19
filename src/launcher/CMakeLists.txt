################################################################################
# Coral Application Launcher
################################################################################

PROJECT( LAUNCHER )

################################################################################
# Build Executable
################################################################################

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/src ${CORE_SOURCE_DIR}/generated )

# Gather sources from the current directory
FILE( GLOB HEADER_FILES *.h )
FILE( GLOB SOURCE_FILES *.cpp )

ADD_EXECUTABLE( launcher ${HEADER_FILES} ${SOURCE_FILES} )
ADD_DEPENDENCIES( launcher lua )

TARGET_LINK_LIBRARIES( launcher core )

CORAL_DEFAULT_TARGET_PROPERTIES( launcher )

SET_TARGET_PROPERTIES( launcher PROPERTIES
	PROJECT_LABEL "Coral Launcher"
)

################################################################################
# Install Rules
################################################################################

# install the launcher executable
INSTALL( TARGETS launcher DESTINATION bin COMPONENT Core )
INSTALL( FILES ${CMAKE_BINARY_DIR}/bin/launcher_debug.pdb DESTINATION bin COMPONENT Core OPTIONAL )
