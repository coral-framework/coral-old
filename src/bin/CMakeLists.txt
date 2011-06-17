################################################################################
# Deployed Executables
################################################################################

project( BIN )

include_directories( ${CMAKE_SOURCE_DIR}/src ${CORE_SOURCE_DIR}/generated )

set( EXTRA_SOURCES
	Utils.h
	${CORE_SOURCE_DIR}/reserved/OS.h
	${CORE_SOURCE_DIR}/reserved/OS.cpp
	${CORE_SOURCE_DIR}/tools/Properties.h
	${CORE_SOURCE_DIR}/tools/Properties.cpp
	${CORE_SOURCE_DIR}/tools/StringTokenizer.h
	${CORE_SOURCE_DIR}/tools/StringTokenizer.cpp
)

################################################################################
# Build the Launcher Back End
################################################################################

add_executable( launcher launcher.cpp Utils.h )
add_dependencies( launcher lua )

target_link_libraries( launcher core )

CORAL_DEFAULT_TARGET_PROPERTIES( launcher )

set_target_properties( launcher PROPERTIES
	PROJECT_LABEL "Executable launcher"
)

################################################################################
# Build the Launcher Front End
################################################################################

add_executable( coral coral.cpp ${EXTRA_SOURCES} )
add_dependencies( coral launcher )

CORAL_DEFAULT_TARGET_PROPERTIES( coral )
set_property( TARGET coral APPEND PROPERTY COMPILE_DEFINITIONS "CORAL_NO_EXPORT" )
set_target_properties( coral PROPERTIES	PROJECT_LABEL "Executable coral" )

################################################################################
# Build the Compiler Front End
################################################################################

add_executable( coralc coralc.cpp ${EXTRA_SOURCES} )
add_dependencies( coralc launcher )

CORAL_DEFAULT_TARGET_PROPERTIES( coralc )
set_property( TARGET coralc APPEND PROPERTY COMPILE_DEFINITIONS "CORAL_NO_EXPORT" )
set_target_properties( coralc PROPERTIES PROJECT_LABEL "Executable coralc" )

################################################################################
# Install Rules
################################################################################

# install the front-end executables (only in release mode)
install( TARGETS coral coralc DESTINATION . CONFIGURATIONS Release RelWithDebInfo COMPONENT Core )

# install the launcher executable
install( TARGETS launcher DESTINATION bin COMPONENT Core )
install( FILES ${CMAKE_BINARY_DIR}/bin/launcher_debug.pdb DESTINATION bin CONFIGURATIONS Debug COMPONENT Core OPTIONAL )
