################################################################################
# About This Directory:
#	tests/moduleB defines a second Coral module for testing module dependency
#	handling and inter-module interactions. This module depends on moduleA.
#	General module tests are contained in the test executable tests_moduleB.
################################################################################

PROJECT( TESTS_MODULEB )

################################################################################
# Build the 'moduleB' module library
################################################################################

# Run the coral compiler on the module
SET( CORAL_PATH
	"${CMAKE_BINARY_DIR}/modules"
	"${CMAKE_SOURCE_DIR}/modules"
	"${CMAKE_SOURCE_DIR}/tests/moduleA"
	"${CMAKE_SOURCE_DIR}/tests/moduleB"
)

CORAL_GENERATE_MODULE( GENERATED_MODULEB_SOURCES moduleB
	co.TypeCreationTransaction
	moduleA.TestInterface
)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/generated )

ADD_LIBRARY( moduleB MODULE EXCLUDE_FROM_ALL ModulePart.h ModulePart.cpp ${GENERATED_MODULEB_SOURCES} )

CORAL_MODULE_TARGET_PROPERTIES( moduleB )

SET_TARGET_PROPERTIES( moduleB PROPERTIES
	PROJECT_LABEL "Module B"
)

TARGET_LINK_LIBRARIES( moduleB core )

################################################################################
# Build the 'tests_moduleB' executable
################################################################################

# Pass the CORAL_PATH as a precompiler definition
CORAL_GET_PATH_STRING( CORAL_PATH_STR )
SET_PROPERTY( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS "CORAL_PATH=\"${CORAL_PATH_STR}\"" )

INCLUDE_DIRECTORIES( ${GTEST_INCLUDE_DIRS} )

# Gather test source files in the current directory
FILE( GLOB TEST_SOURCE_FILES *Tests.cpp )

# Create test executable
ADD_EXECUTABLE( tests_moduleB EXCLUDE_FROM_ALL Main.cpp ${TEST_SOURCE_FILES} )
ADD_DEPENDENCIES( tests_moduleB moduleB )

SET_TARGET_PROPERTIES( tests_moduleB PROPERTIES
	PROJECT_LABEL "Tests - Module B"
)

CORAL_TEST_TARGET_PROPERTIES( tests_moduleB tests_moduleB_EXECUTABLE )

TARGET_LINK_LIBRARIES( tests_moduleB
	core
	${GTEST_LIBRARIES}
	${GFLAGS_LIBRARIES}
)

################################################################################
# Register the test with CTest
################################################################################

FILE( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/output )
ADD_TEST( tests_moduleB ${tests_moduleB_EXECUTABLE} --gtest_output=xml:../output/ModuleBTests.xml )

################################################################################
# If Valgrind is available, add a memory-checking test for tests_moduleB
################################################################################
IF( VALGRIND_COMMAND )
	ADD_TEST( tests_moduleB_MemoryCheck ${VALGRIND_COMMAND}
		--leak-check=full --show-reachable=yes --num-callers=15
		--log-file=${CMAKE_BINARY_DIR}/valgrind-moduleB.log --error-exitcode=13 --suppressions=${CMAKE_SOURCE_DIR}/tests/valgrind.supp ${tests_moduleB_EXECUTABLE} )
ENDIF()

################################################################################
# Source Groups
################################################################################

SOURCE_GROUP( "@Generated" FILES ${GENERATED_MODULEB_SOURCES} )
