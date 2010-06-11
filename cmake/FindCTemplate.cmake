#	Locate Google's CTemplate library (http://code.google.com/p/google-ctemplate/)
#
#	Defines the following variables:
#		CTEMPLATE_FOUND - Found the CTemplate library.
#		CTEMPLATE_INCLUDE_DIRS - Include directories.
#
#	Also defines the library variable below. This contains debug/optimized
#	keywords when a debug library is found.
#		CTEMPLATE_LIBRARIES - libctemplate.
#
#	Accepts the following variables as input:
#		CTEMPLATE_ROOT - (as a CMake or environment variable)
#					The root directory of the CTemplate install prefix.
#

FIND_PATH( CTEMPLATE_INCLUDE_DIR ctemplate/template.h
	HINTS
		$ENV{CTEMPLATE_ROOT}/include
		${CTEMPLATE_ROOT}/include
)
MARK_AS_ADVANCED( CTEMPLATE_INCLUDE_DIR )

FUNCTION( _ctemplate_find_library _name)
	FIND_LIBRARY( ${_name}
		NAMES ${ARGN}
		HINTS
			$ENV{CTEMPLATE_ROOT}
			${CTEMPLATE_ROOT}
		PATH_SUFFIXES
			lib
	)
	MARK_AS_ADVANCED( ${_name} )
ENDFUNCTION()

_ctemplate_find_library( CTEMPLATE_LIBRARY			ctemplate_nothreads ctemplate libctemplate )
_ctemplate_find_library( CTEMPLATE_LIBRARY_DEBUG	libctemplate-debug libctemplated )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( CTemplate DEFAULT_MSG CTEMPLATE_LIBRARY CTEMPLATE_INCLUDE_DIR )

IF( CTEMPLATE_FOUND )
	SET( CTEMPLATE_INCLUDE_DIRS ${CTEMPLATE_INCLUDE_DIR} )
	IF( CTEMPLATE_LIBRARY_DEBUG )
		SET( CTEMPLATE_LIBRARIES optimized ${CTEMPLATE_LIBRARY} debug ${CTEMPLATE_LIBRARY_DEBUG} )
	ELSE()
		SET( CTEMPLATE_LIBRARIES ${CTEMPLATE_LIBRARY} )
	ENDIF()
ENDIF()
