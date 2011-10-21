################################################################################
# Even more tests for the Coral core
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

CORAL_GENERATE_MODULE( _GENERATED_FILES moduleB
	co.ITypeTransaction
	moduleA.TestInterface
)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/generated )

ADD_LIBRARY( moduleB MODULE EXCLUDE_FROM_ALL ModulePart.cpp ${_GENERATED_FILES} )

CORAL_DEFAULT_TARGET_PROPERTIES( moduleB )
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
ADD_DEPENDENCIES( tests_moduleB moduleB lua )

CORAL_DEFAULT_TARGET_PROPERTIES( tests_moduleB )

SET_TARGET_PROPERTIES( tests_moduleB PROPERTIES
	PROJECT_LABEL "Tests - Module B"
)

TARGET_LINK_LIBRARIES( tests_moduleB
	core
	${GTEST_LIBRARIES}
)

################################################################################
# Register the test with CTest
################################################################################
FILE( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/output )
ADD_TEST(
	NAME tests_moduleB
	COMMAND $<TARGET_FILE:tests_moduleB> --gtest_output=xml:../output/TestsModuleB$<CONFIGURATION>.xml
)

################################################################################
# If Valgrind is available, repeat the test checking for memory leaks
################################################################################
IF( VALGRIND_COMMAND )
	ADD_TEST(
		NAME tests_moduleB_MemoryCheck
		COMMAND ${VALGRIND_COMMAND} --leak-check=full --show-reachable=yes --num-callers=15 --dsymutil=yes
		--log-file=${CMAKE_BINARY_DIR}/ValgrindModuleB$<CONFIGURATION>.log --error-exitcode=13
		--suppressions=${CMAKE_SOURCE_DIR}/tests/valgrind.supp $<TARGET_FILE:tests_moduleB>
	)
ENDIF()
