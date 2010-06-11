################################################################################
# About This Directory:
#	src/core/reserved contains public (exported) classes whose usage is somehow
#	limited (or reserved) to advanced users or automatically-generated code,
#	due to the specificity or 'low-levelness' of their functionality.
################################################################################

# Gather sources from the current directory
FILE( GLOB _HEADER_FILES *.h )
FILE( GLOB _SOURCE_FILES *.cpp )

# Export stuff from the current dir
SET_PROPERTY( DIRECTORY ${CORE_SOURCE_DIR} APPEND PROPERTY HEADER_FILES ${_HEADER_FILES} )
SET_PROPERTY( DIRECTORY ${CORE_SOURCE_DIR} APPEND PROPERTY SOURCE_FILES ${_SOURCE_FILES} )
