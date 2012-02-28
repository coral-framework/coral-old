################################################################################
# Tests for the Coral core
################################################################################

project( TESTS_CORE )

################################################################################
# Build the tests_core executable
################################################################################

# Gather files in the current directory
file( GLOB HEADER_FILES *.h )
file( GLOB SOURCE_FILES *.cpp )

# Pass the CORAL_PATH as a precompiler definition
set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
	"CORAL_PATH=\"${CMAKE_SOURCE_DIR}/modules,${CMAKE_CURRENT_SOURCE_DIR}/data/csl\""
	"TESTS_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/data\""
)

include_directories(
	${CORE_SOURCE_DIR}/generated
	${CMAKE_SOURCE_DIR}/src
)

add_executable( tests_core EXCLUDE_FROM_ALL ${HEADER_FILES} ${SOURCE_FILES} )

CORAL_TARGET( tests_core )

set_target_properties( tests_core PROPERTIES
	PROJECT_LABEL "Tests - Core"
)

target_link_libraries( tests_core ${GTEST_LIBRARIES} )

################################################################################
# Register the test with CTest
################################################################################
file( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/output )
add_test(
	NAME tests_core
	COMMAND $<TARGET_FILE:tests_core> --gtest_output=xml:../output/TestsCore$<CONFIGURATION>.xml
)

################################################################################
# If Valgrind is available, repeat the test checking for memory leaks
################################################################################
if( VALGRIND_COMMAND )
	add_test(
		NAME tests_core_MemoryCheck
		COMMAND ${VALGRIND_COMMAND} --leak-check=full --show-reachable=yes --num-callers=15 --dsymutil=yes
			--log-file=${CMAKE_BINARY_DIR}/ValgrindCore$<CONFIGURATION>.log --error-exitcode=13
			--suppressions=${CMAKE_SOURCE_DIR}/tests/valgrind.supp $<TARGET_FILE:tests_core>
	)
endif()

################################################################################
# Source Groups
################################################################################

# Default Source Group
source_group( "Helpers" REGULAR_EXPRESSION ".*" )
source_group( "" FILES "CMakeLists.txt" )

source_group( "Tests" REGULAR_EXPRESSION ".*Tests\\.cpp$" )
