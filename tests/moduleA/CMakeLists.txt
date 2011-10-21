################################################################################
# More tests for the Coral core
################################################################################

PROJECT( TESTS_MODULEA )

################################################################################
# Build the 'moduleA' module
################################################################################

# Run the coral compiler on the module; we also set the CORAL_PATH with
# a repeated CSL dir to check whether it would cause a problem.
SET( CORAL_PATH
	"${CMAKE_BINARY_DIR}/modules"
	"${CMAKE_SOURCE_DIR}/modules"
	"${CMAKE_SOURCE_DIR}/modules"
	"${CMAKE_CURRENT_SOURCE_DIR}"
)
CORAL_GENERATE_MODULE( _GENERATED_FILES moduleA )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/generated )

FILE( GLOB _HEADERS *.h )
FILE( GLOB _SOURCES *Adapter.cpp Test*.cpp *Annotation.cpp )

ADD_LIBRARY( moduleA MODULE EXCLUDE_FROM_ALL ${_HEADERS} ${_SOURCES} ${_GENERATED_FILES} )

CORAL_DEFAULT_TARGET_PROPERTIES( moduleA )
CORAL_MODULE_TARGET_PROPERTIES( moduleA )

SET_TARGET_PROPERTIES( moduleA PROPERTIES
	PROJECT_LABEL "Module A"
)

TARGET_LINK_LIBRARIES( moduleA core )

################################################################################
# Build the test executable
################################################################################

# Pass the CORAL_PATH as a precompiler definition
CORAL_GET_PATH_STRING( CORAL_PATH_STR )
SET_PROPERTY( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS "CORAL_PATH=\"${CORAL_PATH_STR}\"" )

INCLUDE_DIRECTORIES( ${GTEST_INCLUDE_DIRS} )

# Gather test source files in the current directory
FILE( GLOB _TESTS *Tests.cpp )

# Create test executable
ADD_EXECUTABLE( tests_moduleA EXCLUDE_FROM_ALL Main.cpp ${_TESTS} )
ADD_DEPENDENCIES( tests_moduleA moduleA )

CORAL_DEFAULT_TARGET_PROPERTIES( tests_moduleA )

SET_TARGET_PROPERTIES( tests_moduleA PROPERTIES
	PROJECT_LABEL "Tests - Module A"
)

TARGET_LINK_LIBRARIES( tests_moduleA core ${GTEST_LIBRARIES} )

################################################################################
# Register the test
################################################################################
FILE( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/output )
ADD_TEST(
	NAME tests_moduleA
	COMMAND $<TARGET_FILE:tests_moduleA> --gtest_output=xml:../output/TestsModuleA$<CONFIGURATION>.xml
)

################################################################################
# If Valgrind is available, repeat the test checking for memory leaks
################################################################################
IF( VALGRIND_COMMAND )
	ADD_TEST(
		NAME tests_moduleA_MemoryCheck
		COMMAND ${VALGRIND_COMMAND} --leak-check=full --show-reachable=yes --num-callers=15 --dsymutil=yes
		--log-file=${CMAKE_BINARY_DIR}/ValgrindModuleA$<CONFIGURATION>.log --error-exitcode=13
		--suppressions=${CMAKE_SOURCE_DIR}/tests/valgrind.supp $<TARGET_FILE:tests_moduleA>
	)
ENDIF()
