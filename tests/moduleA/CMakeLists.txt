################################################################################
# About This Directory:
#	tests/moduleA defines a Coral module containing extra type definitions
#	and proof-of-concept code for features that were not covered by the core
#	library. It also provides dependencies for moduleB, in order to test module
#	dependency handling. Finally, this dir also contains tests_moduleA, for
#	tests that could not go into tests_core because they required extra types.
################################################################################

PROJECT( TESTS_MODULEA )

################################################################################
# Build the 'moduleA' module library
################################################################################

# Run the coral compiler on the module; we also set the CORAL_PATH with
# a repeated CSL dir to check whether it would cause a problem.
SET( CORAL_PATH
	"${CMAKE_BINARY_DIR}/modules"
	"${CMAKE_SOURCE_DIR}/modules"
	"${CMAKE_SOURCE_DIR}/modules"
	"${CMAKE_CURRENT_SOURCE_DIR}"
)
CORAL_GENERATE_MODULE( GENERATED_MODULEA_SOURCES moduleA )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/generated )

FILE( GLOB HEADER_FILES *.h )
FILE( GLOB SOURCE_FILES *Adapter.cpp *Component.cpp )

ADD_LIBRARY( moduleA MODULE EXCLUDE_FROM_ALL ${HEADER_FILES} ${SOURCE_FILES} ${GENERATED_MODULEA_SOURCES} )

CORAL_DEFAULT_TARGET_PROPERTIES( moduleA )
CORAL_MODULE_TARGET_PROPERTIES( moduleA )

SET_TARGET_PROPERTIES( moduleA PROPERTIES
	PROJECT_LABEL "Module A"
)

TARGET_LINK_LIBRARIES( moduleA core )

################################################################################
# Build the 'tests_moduleA' executable
################################################################################

# Pass the CORAL_PATH as a precompiler definition
CORAL_GET_PATH_STRING( CORAL_PATH_STR )
SET_PROPERTY( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS "CORAL_PATH=\"${CORAL_PATH_STR}\"" )

INCLUDE_DIRECTORIES( ${GTEST_INCLUDE_DIRS} )

# Gather test source files in the current directory
FILE( GLOB TEST_SOURCE_FILES *Tests.cpp )

# Create test executable
ADD_EXECUTABLE( tests_moduleA EXCLUDE_FROM_ALL Main.cpp ${TEST_SOURCE_FILES} )
ADD_DEPENDENCIES( tests_moduleA moduleA )

CORAL_DEFAULT_TARGET_PROPERTIES( tests_moduleA )

SET_TARGET_PROPERTIES( tests_moduleA PROPERTIES
	PROJECT_LABEL "Tests - Module A"
)

TARGET_LINK_LIBRARIES( tests_moduleA
	core
	${GTEST_LIBRARIES}
)

################################################################################
# Register the test with CTest
################################################################################
FILE( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/output )
ADD_TEST(
	NAME tests_moduleA
	COMMAND $<TARGET_FILE:tests_moduleA> --gtest_output=xml:../output/TestsModuleA$<CONFIGURATION>.xml
)

################################################################################
# Source Groups
################################################################################

SOURCE_GROUP( "@Generated" FILES ${GENERATED_MODULEA_SOURCES} )
