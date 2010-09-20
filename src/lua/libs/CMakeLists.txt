###############################################################################
# About This Directory:
#	src/lua/libs contains copies of the following Lua libraries:
#		- lua: Lua 5.2 (stripped of lua.c, luac.c and print.c)
#		- lsf: LuaFileSystem
#	These are compiled into a static library and linked with the Lua Module.
#	All these libraries are licensed under the MIT license (same as Coral).
###############################################################################

PROJECT( LUALIBS C )

IF( UNIX )
	ADD_DEFINITIONS( -fPIC -DLUA_USE_LINUX )
ENDIF()

FILE( GLOB LUA_SOURCES lua/*.c )
FILE( GLOB LFS_SOURCES lfs/*.c )

INCLUDE_DIRECTORIES( lua )

ADD_LIBRARY( lualibs STATIC EXCLUDE_FROM_ALL ${LUA_SOURCES} ${LFS_SOURCES} )

SET_TARGET_PROPERTIES( lualibs PROPERTIES
	PROJECT_LABEL	"Lua Embedded Libs"
)

CORAL_DEFAULT_TARGET_PROPERTIES( lualibs )
