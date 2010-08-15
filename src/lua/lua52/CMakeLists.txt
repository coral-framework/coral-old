###############################################################################
# About This Directory:
#	src/lua/lua52 contains a copy of the Lua 5.2 source files, which are
#	compiled into a static library and linked with the Lua Module.
#	Lua is licensed under the MIT license. For more info: http://www.lua.org/
###############################################################################

PROJECT( LUA52 C )

IF( UNIX )
	ADD_DEFINITIONS( -fPIC -DLUA_USE_LINUX )
ENDIF()

SET( LUA52_CORE_SOURCES
	lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c
	lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c
	ltm.c lundump.c lvm.c lzio.c
)

SET( LUA52_LIB_SOURCES
	lauxlib.c lbaselib.c lcorolib.c lbitlib.c ldblib.c liolib.c lmathlib.c
	loslib.c lstrlib.c ltablib.c loadlib.c linit.c
)

ADD_LIBRARY( lua52 STATIC EXCLUDE_FROM_ALL ${LUA52_CORE_SOURCES} ${LUA52_LIB_SOURCES} )

SET_TARGET_PROPERTIES( lua52 PROPERTIES
	PROJECT_LABEL	"Lua 5.2 Static Library"
)

CORAL_DEFAULT_TARGET_PROPERTIES( lua52 )
