################################################################################
# Deployed Executables
################################################################################

PROJECT( BIN )

ADD_DEFINITIONS( -DCORAL_NO_EXPORT )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/src ${CORE_SOURCE_DIR}/generated )

SET( COMMON_EXTRA_SOURCES Utils.h ${CORE_SOURCE_DIR}/reserved/OS.h ${CORE_SOURCE_DIR}/reserved/OS.cpp )

################################################################################
# Build the Coral Application Launcher Front-End
################################################################################

ADD_EXECUTABLE( coral coral.cpp ${COMMON_EXTRA_SOURCES} )

CORAL_DEFAULT_TARGET_PROPERTIES( coral )

SET_TARGET_PROPERTIES( coral PROPERTIES
	PROJECT_LABEL "Executable coral"
)

################################################################################
# Build the Coral Compiler Front-End
################################################################################

ADD_EXECUTABLE( coralc coralc.cpp ${COMMON_EXTRA_SOURCES} )

CORAL_DEFAULT_TARGET_PROPERTIES( coralc )

SET_TARGET_PROPERTIES( coralc PROPERTIES
	PROJECT_LABEL "Executable coralc"
)

################################################################################
# Build the actual Coral Application Launcher
################################################################################

ADD_EXECUTABLE( launcher launcher.cpp ${COMMON_EXTRA_SOURCES} )
ADD_DEPENDENCIES( launcher lua )

TARGET_LINK_LIBRARIES( launcher core )

CORAL_DEFAULT_TARGET_PROPERTIES( launcher )

SET_TARGET_PROPERTIES( launcher PROPERTIES
	PROJECT_LABEL "Executable launcher"
)

################################################################################
# Install Rules
################################################################################

# install the front-end executables (only in release mode)
INSTALL( TARGETS coral coralc DESTINATION . CONFIGURATIONS Release RelWithDebInfo COMPONENT Core )

# install the launcher executable
INSTALL( TARGETS launcher DESTINATION bin COMPONENT Core )
INSTALL( FILES ${CMAKE_BINARY_DIR}/bin/launcher_debug.pdb DESTINATION bin CONFIGURATIONS Debug COMPONENT Core OPTIONAL )
