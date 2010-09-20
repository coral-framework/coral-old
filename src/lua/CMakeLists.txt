################################################################################
# Lua Module: a two-way bridge between Coral and Lua.
################################################################################

PROJECT( MODULE_LUA )

################################################################################
# Build the 'lua' module library
################################################################################

# Compile the embedded Lua libraries into a static library
ADD_SUBDIRECTORY( libs )

INCLUDE_DIRECTORIES( libs/lua libs/lfs generated ${CORE_SOURCE_DIR}/generated )

FILE( GLOB HEADER_FILES *.h )
FILE( GLOB SOURCE_FILES *.cpp )
FILE( GLOB GENERATED_LUA_SOURCES generated/*.cpp )

ADD_LIBRARY( lua MODULE ${HEADER_FILES} ${SOURCE_FILES} ${GENERATED_LUA_SOURCES} )

CORAL_MODULE_TARGET_PROPERTIES( lua )

SET_TARGET_PROPERTIES( lua PROPERTIES
	PROJECT_LABEL "Module Lua"
)

TARGET_LINK_LIBRARIES( lua lualibs core )

################################################################################
# Source Groups
################################################################################

SOURCE_GROUP( "@Generated" FILES ${GENERATED_LUA_SOURCES} )

################################################################################
# Install Rules
################################################################################

# install shared library
INSTALL( DIRECTORY ${CMAKE_BINARY_DIR}/modules/lua DESTINATION modules COMPONENT "Lua Module" )

# install CSL files
INSTALL( DIRECTORY ${CMAKE_SOURCE_DIR}/modules/lua DESTINATION modules COMPONENT "Lua Module" )
