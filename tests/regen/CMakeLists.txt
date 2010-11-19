################################################################################
# About This Directory:
#	tests/regen re-builds the 'coral' library using freshly regenerated code,
#	produced by the most up-to-date coralc build. It also re-runs all
#	reflection tests.
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
CORAL_GENERATE_MODULE( GENERATED_CORE_SOURCES co )

# Pass the CORAL_PATH as a precompiler definition
CORAL_GET_PATH_STRING( coralPathStr )
SET_PROPERTY( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS "CORAL_PATH=\"${coralPathStr}\"" )

# Get the list of source files for the 'core' library
GET_PROPERTY( CORE_HEADER_FILES DIRECTORY ${CORE_SOURCE_DIR} PROPERTY HEADER_FILES )
GET_PROPERTY( CORE_SOURCE_FILES DIRECTORY ${CORE_SOURCE_DIR} PROPERTY SOURCE_FILES )

INCLUDE_DIRECTORIES(
	${ANTLR3C_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_CURRENT_BINARY_DIR}/generated
	${GTEST_INCLUDE_DIRS}
)

# Create the test executable
ADD_DEFINITIONS( -DBUILDING_CORAL_CORE )
ADD_EXECUTABLE( tests_regen EXCLUDE_FROM_ALL ${CORE_HEADER_FILES} ${CORE_SOURCE_FILES} ${GENERATED_CORE_SOURCES} Main.cpp
	# Select a few tests from tests_core to run again in tests_regen
	../core/AnyTests.cpp
	../core/ReflectorTests.cpp
)

SET_TARGET_PROPERTIES( tests_regen PROPERTIES
	PROJECT_LABEL "Tests - Regeneration"
)

CORAL_TEST_TARGET_PROPERTIES( tests_regen tests_regen_EXECUTABLE )

TARGET_LINK_LIBRARIES( tests_regen
	antlr3c
	${GTEST_LIBRARIES}
)

# Disable warnings for AntLR-generated source files
IF( UNIX )
	SET_PROPERTY(
		SOURCE ${CORE_SOURCE_DIR}/csl/CSLLexer.cpp ${CORE_SOURCE_DIR}/csl/CSLParser.cpp
		PROPERTY COMPILE_FLAGS "-Wno-all -Wno-extra -Wno-unused"
	)
ENDIF()

IF( UNIX )
    TARGET_LINK_LIBRARIES( tests_regen dl )
ENDIF()

################################################################################
# Register the test with CTest
################################################################################

FILE( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/output )
ADD_TEST( tests_regen ${tests_regen_EXECUTABLE} --gtest_output=xml:../output/RegenerationTests.xml )
CORAL_DEFAULT_TEST_PROPERTIES( tests_regen )

################################################################################
# Source Groups
################################################################################

SOURCE_GROUP( "@Core" FILES ${CORE_HEADER_FILES} ${CORE_SOURCE_FILES} )
SOURCE_GROUP( "@Generated" FILES ${GENERATED_CORE_SOURCES} )
