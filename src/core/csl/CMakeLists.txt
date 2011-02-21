################################################################################
# CSL
################################################################################

# Build the embedded ANTLR 3 C runtime as a dependency.
# Altough it's a C library, it's compiled as C++ so we can throw exceptions.
ADD_SUBDIRECTORY( antlr3c )

# Gather sources from the current directory
FILE( GLOB _HEADER_FILES *.h )
FILE( GLOB _SOURCE_FILES *.cpp )

# Export stuff from the current dir
SET_PROPERTY( DIRECTORY ${CORE_SOURCE_DIR} APPEND PROPERTY HEADER_FILES ${_HEADER_FILES} )
SET_PROPERTY( DIRECTORY ${CORE_SOURCE_DIR} APPEND PROPERTY SOURCE_FILES ${_SOURCE_FILES} )
