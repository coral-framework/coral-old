################################################################################
# CSL
################################################################################

# Gather sources from the current directory
FILE( GLOB _HEADER_FILES *.h *.hh )
FILE( GLOB _SOURCE_FILES *.cpp *.cc )

# Export stuff from the current dir
SET_PROPERTY( DIRECTORY ${CORE_SOURCE_DIR} APPEND PROPERTY HEADER_FILES ${_HEADER_FILES} )
SET_PROPERTY( DIRECTORY ${CORE_SOURCE_DIR} APPEND PROPERTY SOURCE_FILES ${_SOURCE_FILES} )
