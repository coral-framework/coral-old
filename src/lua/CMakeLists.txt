################################################################################
# Lua Module (fully integrates Coral and Lua)
################################################################################

PROJECT( MODULE_LUA )

################################################################################
# Build the 'lua' module library
################################################################################

# Compile the embedded Lua libraries into a static library
ADD_SUBDIRECTORY( libs )

INCLUDE_DIRECTORIES( libs libs/lua generated ${CORE_SOURCE_DIR}/generated )

FILE( GLOB HEADER_FILES *.h )
FILE( GLOB SOURCE_FILES *.cpp )

ADD_LIBRARY( lua MODULE ${HEADER_FILES} ${SOURCE_FILES} "generated/__AllInOne.cpp" )

CORAL_DEFAULT_TARGET_PROPERTIES( lua )
CORAL_MODULE_TARGET_PROPERTIES( lua )

SET_TARGET_PROPERTIES( lua PROPERTIES
	PROJECT_LABEL "Module Lua"
)

TARGET_LINK_LIBRARIES( lua lualibs core )

################################################################################
# Custom target to re-generate the Lua module files using the Coral compiler
################################################################################

ADD_CUSTOM_TARGET( lua_regen
	COMMAND ${CORAL_LAUNCHER} -p "${CMAKE_SOURCE_DIR}/modules,${CMAKE_BINARY_DIR}/modules"
		--no-abi-checks lua.Launcher co.compiler.cli -g lua
	DEPENDS ${CORAL_LAUNCHER}
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
	COMMENT "Forcing regeneration of the Lua module files..."
)

################################################################################
# Install Rules
################################################################################

# install shared library
INSTALL( TARGETS lua DESTINATION modules/lua COMPONENT "Lua Module" )
INSTALL( FILES ${CMAKE_BINARY_DIR}/modules/lua/lua_debug.pdb DESTINATION modules/lua CONFIGURATIONS Debug COMPONENT "Lua Module" OPTIONAL )

# install CSL files
INSTALL( DIRECTORY ${CMAKE_SOURCE_DIR}/modules/lua DESTINATION modules CONFIGURATIONS Release RelWithDebInfo COMPONENT "Lua Module" )
