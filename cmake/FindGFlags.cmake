#	Locate Google's commandline Flags library (http://code.google.com/p/google-gflags/)
#
#	Defines the following variables:
#		GFLAGS_FOUND - Found the GFlags library.
#		GFLAGS_INCLUDE_DIRS - Include directories.
#
#	Also defines the library variable below. This contains debug/optimized
#	keywords when a debug library is found.
#		GFLAGS_LIBRARIES - libgflags.
#
#	Accepts the following variables as input:
#		GFLAGS_ROOT - (as a CMake or environment variable)
#					The root directory of the GFlags install prefix.
#

FIND_PATH( GFLAGS_INCLUDE_DIR gflags/gflags.h
	HINTS
		$ENV{GFLAGS_ROOT}/include
		${GFLAGS_ROOT}/include
)
MARK_AS_ADVANCED( GFLAGS_INCLUDE_DIR )

FUNCTION( _gflags_find_library _name)
	FIND_LIBRARY( ${_name}
		NAMES ${ARGN}
		HINTS
			$ENV{GFLAGS_ROOT}
			${GFLAGS_ROOT}
		PATH_SUFFIXES
			lib
	)
	MARK_AS_ADVANCED( ${_name} )
ENDFUNCTION()

_gflags_find_library( GFLAGS_LIBRARY		gflags libgflags )
_gflags_find_library( GFLAGS_LIBRARY_DEBUG	gflagsd libgflagsd )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( GFlags DEFAULT_MSG GFLAGS_LIBRARY GFLAGS_INCLUDE_DIR )

IF( GFLAGS_FOUND )
	SET( GFLAGS_INCLUDE_DIRS ${GFLAGS_INCLUDE_DIR} )
	IF( GFLAGS_LIBRARY_DEBUG )
		SET( GFLAGS_LIBRARIES optimized ${GFLAGS_LIBRARY} debug ${GFLAGS_LIBRARY_DEBUG} )
	ELSE()
		SET( GFLAGS_LIBRARIES ${GFLAGS_LIBRARY} )
	ENDIF()
ENDIF()
