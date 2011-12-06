################################################################################
# Tests for the Coral core
################################################################################

PROJECT( TESTS_CORE )

################################################################################
# Build the tests_core executable
################################################################################

# Gather files in the current directory
FILE( GLOB HEADER_FILES *.h )
FILE( GLOB SOURCE_FILES *.cpp )

# Pass the CORAL_PATH as a precompiler definition
SET_PROPERTY( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
	"CORAL_PATH=\"${CMAKE_SOURCE_DIR}/modules,${CMAKE_CURRENT_SOURCE_DIR}/data/csl\""
	"TESTS_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/data\""
)

INCLUDE_DIRECTORIES(
	${CORE_SOURCE_DIR}/generated
	${CMAKE_SOURCE_DIR}/src
	${GTEST_INCLUDE_DIRS}
)

ADD_EXECUTABLE( tests_core EXCLUDE_FROM_ALL ${HEADER_FILES} ${SOURCE_FILES} )

CORAL_TARGET( tests_core )

SET_TARGET_PROPERTIES( tests_core PROPERTIES
	PROJECT_LABEL "Tests - Core"
)

TARGET_LINK_LIBRARIES( tests_core ${GTEST_LIBRARIES} )

################################################################################
# Register the test with CTest
################################################################################
FILE( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/output )
ADD_TEST(
	NAME tests_core
	COMMAND $<TARGET_FILE:tests_core> --gtest_output=xml:../output/TestsCore$<CONFIGURATION>.xml
)

################################################################################
# If Valgrind is available, repeat the test checking for memory leaks
################################################################################
IF( VALGRIND_COMMAND )
	ADD_TEST(
		NAME tests_core_MemoryCheck
		COMMAND ${VALGRIND_COMMAND} --leak-check=full --show-reachable=yes --num-callers=15 --dsymutil=yes
			--log-file=${CMAKE_BINARY_DIR}/ValgrindCore$<CONFIGURATION>.log --error-exitcode=13
			--suppressions=${CMAKE_SOURCE_DIR}/tests/valgrind.supp $<TARGET_FILE:tests_core>
	)
ENDIF()

################################################################################
# Source Groups
################################################################################

# Default Source Group
SOURCE_GROUP( "Helpers" REGULAR_EXPRESSION ".*" )
SOURCE_GROUP( "" FILES "CMakeLists.txt" )

SOURCE_GROUP( "Tests" REGULAR_EXPRESSION ".*Tests\\.cpp$" )
