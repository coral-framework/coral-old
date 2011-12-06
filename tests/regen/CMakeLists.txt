################################################################################
# Tests the code generation rules applied by the Coral compiler
################################################################################

PROJECT( TESTS_REGEN )

################################################################################
# Build the tests_regen executable
################################################################################

# Re-generate code for the 'co' module
SET( CORAL_PATH
	"${CMAKE_BINARY_DIR}/modules"
	"${CMAKE_SOURCE_DIR}/modules"
)

SET( CORAL_LAUNCHER_FLAGS "--no-abi-checks" )
CORAL_GENERATE_MODULE( _GENERATED_FILES co )
SET( CORAL_LAUNCHER_FLAGS "" )

# Pass the CORAL_PATH as a precompiler definition
CORAL_GET_PATH_STRING( coralPathStr )
SET_PROPERTY( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS "CORAL_PATH=\"${coralPathStr}\"" )

# Get the list of source files for the 'core' library
GET_PROPERTY( CORE_HEADER_FILES DIRECTORY ${CORE_SOURCE_DIR} PROPERTY HEADER_FILES )
GET_PROPERTY( CORE_SOURCE_FILES DIRECTORY ${CORE_SOURCE_DIR} PROPERTY SOURCE_FILES )

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_CURRENT_BINARY_DIR}/generated
	${GTEST_INCLUDE_DIRS}
)

# Create the test executable
ADD_DEFINITIONS( -DBUILDING_CORAL_CORE )
ADD_EXECUTABLE( tests_regen EXCLUDE_FROM_ALL ${CORE_HEADER_FILES} ${CORE_SOURCE_FILES} ${_GENERATED_FILES} Main.cpp
	# Select a few tests from tests_core to run again in tests_regen
	../core/AnyTests.cpp
	../core/ReflectorTests.cpp
)

CORAL_TARGET_PROPERTIES( tests_regen )

SET_TARGET_PROPERTIES( tests_regen PROPERTIES
	PROJECT_LABEL "Tests - Regeneration"
)

TARGET_LINK_LIBRARIES( tests_regen ${GTEST_LIBRARIES} )

if( NOT MSVC )
	# Disable warnings for auto-generated code
	set_property( SOURCE "${CORE_SOURCE_DIR}/csl/scanner.cc" APPEND PROPERTY COMPILE_FLAGS "-Wno-unused-parameter" )
endif()

IF( UNIX )
    TARGET_LINK_LIBRARIES( tests_regen dl )
ENDIF()

################################################################################
# Register the test with CTest
################################################################################
FILE( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/output )
ADD_TEST(
	NAME tests_regen
	COMMAND $<TARGET_FILE:tests_regen> --gtest_output=xml:../output/TestsRegeneration$<CONFIGURATION>.xml
)

################################################################################
# Source Groups
################################################################################

SOURCE_GROUP( "@Core" FILES ${CORE_HEADER_FILES} ${CORE_SOURCE_FILES} )
