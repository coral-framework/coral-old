#	Locate Google's Logging library (http://code.google.com/p/google-glog/)
#
#	Defines the following variables:
#		GLOG_FOUND - Found the GLog library.
#		GLOG_INCLUDE_DIRS - Include directories.
#
#	Also defines the library variable below. This contains debug/optimized
#	keywords when a debug library is found.
#		GLOG_LIBRARIES - libglog.
#
#	Accepts the following variables as input:
#		GLOG_ROOT - (as a CMake or environment variable)
#					The root directory of the GLog install prefix.
#

FIND_PATH( GLOG_INCLUDE_DIR glog/logging.h
	HINTS
		$ENV{GLOG_ROOT}/include
		${GLOG_ROOT}/include
)
MARK_AS_ADVANCED( GLOG_INCLUDE_DIR )

FUNCTION( _glog_find_library _name)
	FIND_LIBRARY( ${_name}
		NAMES ${ARGN}
		HINTS
			$ENV{GLOG_ROOT}
			${GLOG_ROOT}
		PATH_SUFFIXES
			lib
	)
	MARK_AS_ADVANCED( ${_name} )
ENDFUNCTION()

_glog_find_library( GLOG_LIBRARY			glog libglog )
_glog_find_library( GLOG_LIBRARY_DEBUG		glogd libglogd )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( GLog DEFAULT_MSG GLOG_LIBRARY GLOG_INCLUDE_DIR )

IF( GLOG_FOUND )
	SET( GLOG_INCLUDE_DIRS ${GLOG_INCLUDE_DIR} )
	IF( GLOG_LIBRARY_DEBUG )
		SET( GLOG_LIBRARIES optimized ${GLOG_LIBRARY} debug ${GLOG_LIBRARY_DEBUG} )
	ELSE()
		SET( GLOG_LIBRARIES ${GLOG_LIBRARY} )
	ENDIF()
ENDIF()
