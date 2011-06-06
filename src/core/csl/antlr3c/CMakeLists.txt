###############################################################################
# About This Directory:
#	src/core/csl/antlr3c contains a copy of the ANTLR 3 C runtime library.
#	It is compiled as C++ into a static library and linked with the Coral core.
#	ANTLR is licensed under the BSD license. See http://www.antlr.org/
###############################################################################

PROJECT( ANTLR3C )

IF( UNIX )
	ADD_DEFINITIONS( -fPIC )
ENDIF()

IF( NOT MSVC )
	# Disable GCC warnings for the antlr3c library
	ADD_DEFINITIONS( -Wno-all -Wno-extra -Wno-unused -Wno-write-strings -Wno-cast-qual -fpermissive )
ENDIF()

# Gather sources from the current directory
FILE( GLOB _HEADER_FILES include/*.h )
FILE( GLOB _SOURCE_FILES src/*.c )

ADD_LIBRARY( antlr3c STATIC EXCLUDE_FROM_ALL ${_HEADER_FILES} ${_SOURCE_FILES} )

# Forces compilation of the C source files as C++
SET_SOURCE_FILES_PROPERTIES( ${_SOURCE_FILES} PROPERTIES LANGUAGE "CXX" )

INCLUDE_DIRECTORIES( include )

SET_TARGET_PROPERTIES( antlr3c PROPERTIES
	PROJECT_LABEL	"ANTLR 3 C Runtime Library"
)

CORAL_DEFAULT_TARGET_PROPERTIES( antlr3c )
