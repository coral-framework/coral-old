/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "MappingGenerator.h"
#include "StreamEmitter.h"
#include <co/Type.h>
#include <co/Namespace.h>
#include <ctemplate/template.h>

MappingGenerator::MappingGenerator( const std::string& mappingsDir )
	: _mappingsDir( mappingsDir )
{
	// empty
}

void MappingGenerator::generate( co::Type* type, ctemplate::TemplateDictionary* dictionary )
{
	assert( type );
	
	const char* typeTemplateName = NULL;
	switch( type->getKind() )
	{
	case co::TK_ENUM:			typeTemplateName = "mapping_enum.tpl";			break;
	case co::TK_EXCEPTION:		typeTemplateName = "mapping_exception.tpl";		break;
	case co::TK_STRUCT:			typeTemplateName = "mapping_struct.tpl";		break;
	case co::TK_NATIVECLASS:	typeTemplateName = "mapping_nativeclass.tpl";	break;
	case co::TK_INTERFACE:		typeTemplateName = "mapping_interface.tpl";		break;
	default:
		assert( false );
	}

	std::string dir;
	computeDir( type, dir );

	FileStreamEmitter fileStream( dir, type->getName() + ".h" );
	fileStream.expand( "mapping_header.tpl", dictionary );
	fileStream.expand( typeTemplateName, dictionary );
	fileStream.expand( "mapping_footer.tpl", dictionary );
}

void MappingGenerator::computeDir( co::Type* type, std::string& dir )
{
	const std::string& ns = type->getNamespace()->getFullName();
	
	unsigned int mappingsDirLen = _mappingsDir.length();
	unsigned int totalLen = mappingsDirLen + ns.length() + 1;

	dir.reserve( totalLen + 1 );
	dir = _mappingsDir;
	dir += '/';
	dir += ns;

	for( unsigned int i = mappingsDirLen + 1; i < totalLen; ++i )
		if( dir[i] == '.' )
			dir[i] = '/';
}
