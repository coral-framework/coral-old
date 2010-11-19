################################################################################
# Coral Core Library
################################################################################

PROJECT( CORE )

#################################################################################
# Generate <co/Config.h>
#################################################################################

SET( CORAL_POINTER_SIZE ${CMAKE_SIZEOF_VOID_P} )

INCLUDE( TestBigEndian )
TEST_BIG_ENDIAN( IS_BIG_ENDIAN )

IF( IS_BIG_ENDIAN )
	SET( CORAL_BYTE_ORDER "CORAL_BIG_ENDIAN" )
ELSE()
	SET( CORAL_BYTE_ORDER "CORAL_LITTLE_ENDIAN" )
ENDIF()

CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in ${CMAKE_BINARY_DIR}/include/co/Config.h )

#################################################################################
# Copy public headers to ${CMAKE_BINARY_DIR}/include/co
#################################################################################

FILE( GLOB CORAL_PUBLIC_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} reserved/*.h )

SET( CORAL_PUBLIC_HEADERS
	Any.h
	ArrayRange.h
	Coral.h
	Endian.h
	Exception.h
	Interface.h
	Platform.h
	RefPtr.h
	RefVector.h
	TypeKind.h
	TypeTraits.h
	${CORAL_PUBLIC_HEADERS}
)

FOREACH( PUBLIC_HEADER ${CORAL_PUBLIC_HEADERS} )
	CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/${PUBLIC_HEADER} ${CMAKE_BINARY_DIR}/include/co/${PUBLIC_HEADER} COPYONLY )
ENDFOREACH()

################################################################################
# Process Subdirectories
################################################################################
ADD_SUBDIRECTORY( csl )
ADD_SUBDIRECTORY( reserved )
ADD_SUBDIRECTORY( tools )

################################################################################
# Build Library
################################################################################

# Gather sources from the current directory
FILE( GLOB _HEADER_FILES *.h )
FILE( GLOB _SOURCE_FILES *.cpp )

# Add stuff from the current dir
SET_PROPERTY( DIRECTORY ${CORE_SOURCE_DIR} APPEND PROPERTY HEADER_FILES ${_HEADER_FILES} )
SET_PROPERTY( DIRECTORY ${CORE_SOURCE_DIR} APPEND PROPERTY SOURCE_FILES ${_SOURCE_FILES} )

# Extract properties gathered from all subdirs
GET_PROPERTY( HEADER_FILES DIRECTORY PROPERTY HEADER_FILES )
GET_PROPERTY( SOURCE_FILES DIRECTORY PROPERTY SOURCE_FILES )

# Gather files from the "generated" dir
FILE( GLOB MAPPING_HEADER_FILES generated/co/*.h )
FILE( GLOB GENERATED_MODULE_FILES generated/*.h generated/*.cpp )

INCLUDE_DIRECTORIES(
	${ANTLR3C_SOURCE_DIR}/include
	generated
)

ADD_DEFINITIONS( -DBUILDING_CORAL_CORE )

ADD_LIBRARY( core SHARED ${MAPPING_HEADER_FILES} ${GENERATED_MODULE_FILES} ${HEADER_FILES} ${SOURCE_FILES} )

SET_TARGET_PROPERTIES( core PROPERTIES
	OUTPUT_NAME   "coral"
	PROJECT_LABEL "Coral Library"
)

CORAL_DEFAULT_TARGET_PROPERTIES( core )

IF( NOT MSVC )
	# Disable warnings for AntLR-generated source files
	SET_PROPERTY( SOURCE csl/CSLLexer.cpp csl/CSLParser.cpp PROPERTY COMPILE_FLAGS "-Wno-all -Wno-extra -Wno-unused" )
ENDIF()

TARGET_LINK_LIBRARIES( core	antlr3c )

IF( UNIX )
    TARGET_LINK_LIBRARIES( core dl )
ENDIF()

################################################################################
# Source Groups
################################################################################

# Default Source Group
SOURCE_GROUP( "Core" REGULAR_EXPRESSION .* )
SOURCE_GROUP( "" FILES CMakeLists.txt )

# Generated Files
SOURCE_GROUP( "@Mappings" FILES ${MAPPING_HEADER_FILES} )
SOURCE_GROUP( "@Generated" FILES ${GENERATED_MODULE_FILES} )

# Module System Files
FILE( GLOB moduleFiles Module*.* )
SOURCE_GROUP( "Module" FILES ${moduleFiles} )

# Reflection System Files
FILE( GLOB reflectionFiles Any* *Reflector* )
SOURCE_GROUP( "Reflection" FILES ${reflectionFiles} )

# Type System Files
FILE( GLOB typeFiles *Type*.* *Info.* AttributeAndMethodContainer.* MethodBuilder.* Namespace.* SignatureCalculator.* )
SOURCE_GROUP( "Type" FILES ${typeFiles} )

# CSL Loading Files
FILE( GLOB cslFiles csl/*.h csl/*.cpp )
SOURCE_GROUP( "CSL" FILES ${cslFiles} )

# 'Reserved' Public Classes
FILE( GLOB reservedFiles reserved/*.h reserved/*.cpp )
SOURCE_GROUP( "Reserved" FILES ${reservedFiles} )

# Internal Utility Classes (Tools)
FILE( GLOB toolsFiles tools/*.h tools/*.cpp )
SOURCE_GROUP( "Tools" FILES ${toolsFiles} )

################################################################################
# Install Rules
################################################################################

# install shared library
INSTALL( TARGETS core DESTINATION lib COMPONENT Core )
INSTALL( FILES ${CMAKE_BINARY_DIR}/bin/coral_debug.pdb DESTINATION lib COMPONENT Core OPTIONAL )

# install CMake package
INSTALL( FILES ${CMAKE_SOURCE_DIR}/cmake/FindCoral.cmake DESTINATION cmake COMPONENT Core )

# install public headers
INSTALL( DIRECTORY ${CMAKE_BINARY_DIR}/include DESTINATION . COMPONENT Core )

# install CSL files
INSTALL( DIRECTORY ${CMAKE_SOURCE_DIR}/modules/co DESTINATION modules COMPONENT Core )
