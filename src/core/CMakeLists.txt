################################################################################
# Coral Core Library
################################################################################

project( CORE )

#################################################################################
# Generate <co/Config.h>
#################################################################################

set( CORAL_POINTER_SIZE ${CMAKE_SIZEOF_VOID_P} )

include( TestBigEndian )
TEST_BIG_ENDIAN( IS_BIG_ENDIAN )

if( IS_BIG_ENDIAN )
	set( CORAL_BYTE_ORDER "CORAL_BIG_ENDIAN" )
else()
	set( CORAL_BYTE_ORDER "CORAL_LITTLE_ENDIAN" )
endif()

configure_file( Config.h.in "${CMAKE_BINARY_DIR}/include/co/Config.h" )

#################################################################################
# Copy public headers to ${CMAKE_BINARY_DIR}/include/co
#################################################################################

file( GLOB CORAL_PUBLIC_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} reserved/*.h )

list( APPEND CORAL_PUBLIC_HEADERS
	Any.h
	Coral.h
	Endian.h
	Exception.h
	IService.h
	Log.h
	Platform.h
	Range.h
	RefPtr.h
	RefVector.h
	TypeKind.h
	TypeTraits.h
)

foreach( PUBLIC_HEADER ${CORAL_PUBLIC_HEADERS} )
	configure_file( ${PUBLIC_HEADER} "${CMAKE_BINARY_DIR}/include/co/${PUBLIC_HEADER}" COPYONLY )
endforeach()

################################################################################
# Process Subdirectories
################################################################################

add_subdirectory( csl )
add_subdirectory( reserved )
add_subdirectory( types )
add_subdirectory( utils )

################################################################################
# Build Library
################################################################################

# Gather sources from the current directory
file( GLOB _HEADER_FILES *.h )
file( GLOB _SOURCE_FILES *.cpp )

# Add stuff from the current dir
set_property( DIRECTORY ${CORE_SOURCE_DIR} APPEND PROPERTY HEADER_FILES ${_HEADER_FILES} )
set_property( DIRECTORY ${CORE_SOURCE_DIR} APPEND PROPERTY SOURCE_FILES ${_SOURCE_FILES} )

# Extract properties gathered from all subdirs
GET_PROPERTY( HEADER_FILES DIRECTORY PROPERTY HEADER_FILES )
GET_PROPERTY( SOURCE_FILES DIRECTORY PROPERTY SOURCE_FILES )

# Gather files from the "generated" dir
file( GLOB MAPPING_HEADER_FILES generated/co/*.h )
file( GLOB GENERATED_MODULE_FILES generated/*.h generated/*.cpp )

include_directories( generated )

add_definitions( -DBUILDING_CORAL_CORE )

add_library( core SHARED
	${MAPPING_HEADER_FILES}
	${GENERATED_MODULE_FILES}
	${HEADER_FILES}
	${SOURCE_FILES}
	"csl/CSL.l"
	"csl/CSL.y"
)

if( NOT MSVC )
	# Disable warnings for auto-generated code
	set_property( SOURCE "csl/scanner.cc" APPEND PROPERTY COMPILE_FLAGS "-Wno-unused-parameter" )
endif()

set_target_properties( core PROPERTIES
	OUTPUT_NAME   "coral"
	PROJECT_LABEL "Coral Library"
)

CORAL_TARGET_PROPERTIES( core )

if( UNIX )
    target_link_libraries( core dl )
endif()

################################################################################
# Source Groups
################################################################################

source_group( "Core" REGULAR_EXPRESSION ".*" )
source_group( "" FILES "CMakeLists.txt" )

source_group( "@Generated" FILES ${GENERATED_MODULE_FILES} )
source_group( "@Generated\\co" FILES ${MAPPING_HEADER_FILES} )

file( GLOB moduleFiles Module*.* )
source_group( "Modules" FILES ${moduleFiles} )

file( GLOB typeFiles Annotations* *Builder* Namespace* *Reflector* Signature* Type* Uuid* )
source_group( "Types" FILES ${typeFiles} )

file( GLOB cslFiles csl/*.h csl/*.cpp csl/*.hh csl/*.cc )
source_group( "Types\\CSL" FILES TypeLoader.h TypeLoader.cpp csl/CSL.l csl/CSL.y ${cslFiles} )

file( GLOB typesFiles Annotations.* types/*.h types/*.cpp )
source_group( "Types\\Descriptors" FILES ${typesFiles} )

file( GLOB reservedFiles reserved/*.h reserved/*.cpp )
source_group( "Reserved" FILES ${reservedFiles} )

file( GLOB utilsFiles utils/*.h utils/*.cpp )
SOURCE_GROUP( "Utility" FILES ${utilsFiles} )

################################################################################
# Install Rules
################################################################################

# install shared library
install( TARGETS core DESTINATION lib COMPONENT Core )
install( FILES ${CMAKE_BINARY_DIR}/bin/coral_debug.pdb DESTINATION lib CONFIGURATIONS Debug COMPONENT Core OPTIONAL )

# install CMake package
install( FILES ${CMAKE_SOURCE_DIR}/cmake/FindCoral.cmake DESTINATION cmake CONFIGURATIONS Release RelWithDebInfo COMPONENT Core )

# install public headers
install( DIRECTORY ${CMAKE_BINARY_DIR}/include DESTINATION . CONFIGURATIONS Release RelWithDebInfo COMPONENT Core )

# install CSL files
install( DIRECTORY ${CMAKE_SOURCE_DIR}/modules/co DESTINATION modules CONFIGURATIONS Release RelWithDebInfo COMPONENT Core )
