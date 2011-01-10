################################################################################
# About This Directory:
#	tests/lua provides a simple test executable for testing Coral modules and
#	components implemented in Lua. It also contains a set of proof-of-concept
#	Lua modules and components in the 'modules' subdir, all of which are
#	tested by the tests_lua executable.
################################################################################

PROJECT( TESTS_LUA )

################################################################################
# Build the 'tests_lua' executable
################################################################################

# Generate the necessary mappings for building the application
SET( CORAL_PATH
	"${CMAKE_BINARY_DIR}/modules"
	"${CMAKE_SOURCE_DIR}/modules"
	"${CMAKE_SOURCE_DIR}/tests/moduleA"
	"${CMAKE_SOURCE_DIR}/tests/moduleB"
	"${CMAKE_SOURCE_DIR}/tests/core/data/csl"
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

CORAL_GENERATE_MAPPINGS( GENERATED_HEADERS co.System )

# Pass the CORAL_PATH as a precompiler definition
CORAL_GET_PATH_STRING( CORAL_PATH_STR )
SET_PROPERTY( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS "CORAL_PATH=\"${CORAL_PATH_STR}\"" )

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}/generated
	${GTEST_INCLUDE_DIRS}
)

# Create test executable
ADD_EXECUTABLE( tests_lua EXCLUDE_FROM_ALL Main.cpp LuaTests.cpp ${GENERATED_HEADERS} )
ADD_DEPENDENCIES( tests_lua lua moduleA )

CORAL_DEFAULT_TARGET_PROPERTIES( tests_lua )

SET_TARGET_PROPERTIES( tests_lua PROPERTIES
	PROJECT_LABEL "Tests - Lua"
)

TARGET_LINK_LIBRARIES( tests_lua
	core
	${GTEST_LIBRARIES}
)

################################################################################
# Register the test with CTest
################################################################################
FILE( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/output )
ADD_TEST(
	NAME tests_lua
	COMMAND $<TARGET_FILE:tests_lua> --gtest_output=xml:../output/TestsLua$<CONFIGURATION>.xml
)

################################################################################
# If Valgrind is available, repeat the test checking for memory leaks
################################################################################
IF( VALGRIND_COMMAND )
	ADD_TEST(
		NAME tests_lua_MemoryCheck
		COMMAND ${VALGRIND_COMMAND} --leak-check=full --show-reachable=yes --num-callers=30 --dsymutil=yes
			--log-file=${CMAKE_BINARY_DIR}/ValgrindLua$<CONFIGURATION>.log --error-exitcode=13
			--suppressions=${CMAKE_SOURCE_DIR}/tests/valgrind.supp $<TARGET_FILE:tests_lua>
	)
ENDIF()

################################################################################
# Source Groups
################################################################################

SOURCE_GROUP( "@Mappings" FILES ${GENERATED_HEADERS} )
