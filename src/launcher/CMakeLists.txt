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

SET_TARGET_PROPERTIES( launcher PROPERTIES
	OUTPUT_NAME   "coral"
	PROJECT_LABEL "Coral Launcher"
)

CORAL_DEFAULT_TARGET_PROPERTIES( launcher )

################################################################################
# Install Rules
################################################################################

# install the launcher executable
IF( NOT WIN32 )
	INSTALL( TARGETS launcher DESTINATION bin COMPONENT Core )
ELSE()
	# on windows use a different approach, to copy the manifest and pdb files as well
	FILE( GLOB _binFiles
		"${CMAKE_BINARY_DIR}/bin/coral*.exe"
		"${CMAKE_BINARY_DIR}/bin/coral*.pdb"
	)
	INSTALL( FILES ${_binFiles} DESTINATION bin COMPONENT Core )
ENDIF()
