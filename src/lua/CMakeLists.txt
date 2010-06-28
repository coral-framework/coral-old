################################################################################
# Lua Module: a two-way bridge between Coral and Lua.
################################################################################

PROJECT( MODULE_LUA )

################################################################################
# Build the 'lua' module library
################################################################################

# Build the embedded Lua 5.2 library as a dependency
ADD_SUBDIRECTORY( lua52 )

# Run the coral compiler on the module
SET( CORAL_PATH "${CMAKE_SOURCE_DIR}/modules" )
CORAL_GENERATE( GENERATED_LUA_SOURCES lua )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/generated )

FILE( GLOB HEADER_FILES *.h )
FILE( GLOB SOURCE_FILES *.cpp )

ADD_LIBRARY( lua MODULE ${HEADER_FILES} ${SOURCE_FILES} ${GENERATED_LUA_SOURCES} )

CORAL_MODULE_TARGET_PROPERTIES( lua )

SET_TARGET_PROPERTIES( lua PROPERTIES
	PROJECT_LABEL "Module Lua"
)

TARGET_LINK_LIBRARIES( lua lua52 core )

################################################################################
# Source Groups
################################################################################

SOURCE_GROUP( "@Generated" FILES ${GENERATED_LUA_SOURCES} )