################################################################################
# Coral CSL Compiler
################################################################################

PROJECT( COMPILER )

################################################################################
# Build Compiler
################################################################################

# Google's ctemplate library is required.
FIND_PACKAGE( CTemplate REQUIRED )

# Gather sources from the current directory
FILE( GLOB HEADER_FILES *.h )
FILE( GLOB SOURCE_FILES *.cpp )

SET_PROPERTY( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
	"COMPILER_TEMPLATES_DIR=\"${CMAKE_SOURCE_DIR}/src/compiler/templates\""
)

ADD_EXECUTABLE( coralc ${HEADER_FILES} ${SOURCE_FILES} )

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/src
	${CORE_SOURCE_DIR}/generated
	${CTEMPLATE_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES( coralc
	core
	${GFLAGS_LIBRARIES}
	${CTEMPLATE_LIBRARIES}
)

SET_TARGET_PROPERTIES( coralc PROPERTIES
	PROJECT_LABEL "Coral Compiler"
)

CORAL_DEFAULT_TARGET_PROPERTIES( coralc )

################################################################################
# Install Rules
################################################################################

# install the compiler executable
INSTALL( TARGETS coralc DESTINATION bin COMPONENT Core )
