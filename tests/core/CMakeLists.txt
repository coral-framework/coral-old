################################################################################
# About This Directory:
#	tests/core contains unit tests for the Coral core library.
################################################################################

PROJECT( TESTS_CORE )

################################################################################
# Build the tests_core executable
################################################################################

# Process subdirectories
ADD_SUBDIRECTORY( tools )

# Extract properties gathered from all subdirs
GET_PROPERTY( HEADER_FILES DIRECTORY PROPERTY HEADER_FILES )
GET_PROPERTY( SOURCE_FILES DIRECTORY PROPERTY SOURCE_FILES )

# Gather files in the current directory
FILE( GLOB _HEADER_FILES *.h )
FILE( GLOB _SOURCE_FILES *.cpp )

# Add source files in the current directory
SET( HEADER_FILES ${_HEADER_FILES} ${HEADER_FILES} )
SET( SOURCE_FILES ${_SOURCE_FILES} ${SOURCE_FILES} )

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

SET_TARGET_PROPERTIES( tests_core PROPERTIES
	PROJECT_LABEL "Tests - Core"
)

TARGET_LINK_LIBRARIES( tests_core
	core
	${GTEST_LIBRARIES}
)

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

FILE( GLOB testsFiles ${CMAKE_CURRENT_SOURCE_DIR}/*Tests.cpp )
SOURCE_GROUP( "Core Tests" FILES ${testsFiles} )

FILE( GLOB toolsFiles ${CMAKE_CURRENT_SOURCE_DIR}/tools/*.cpp )
SOURCE_GROUP( "Tools Tests" FILES ${toolsFiles} )

FILE( GLOB miscFiles ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )
LIST( REMOVE_ITEM miscFiles ${testsFiles} )

SOURCE_GROUP( "@Misc" FILES ${miscFiles} )
