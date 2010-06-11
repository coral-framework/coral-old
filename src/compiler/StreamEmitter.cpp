/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "StreamEmitter.h"
#include <co/CoreException.h>
#include <core/tools/Dir.h>
#include <ctemplate/template.h>
#include <sstream>

// ------ StreamEmitter ------------------------------------------------------

StreamEmitter::StreamEmitter( std::ostream& ostream ) : _ostream( ostream )
{
	// empty
}

StreamEmitter::~StreamEmitter()
{
	// empty
}

void StreamEmitter::expand( ctemplate::Template* tpl, const ctemplate::TemplateDictionaryInterface* data )
{
	assert( tpl && data );
	tpl->Expand( this, data );
}

void StreamEmitter::expand( const char* templateName, const ctemplate::TemplateDictionaryInterface* data )
{
	ctemplate::Template* tpl = ctemplate::Template::GetTemplate( templateName, ctemplate::STRIP_BLANK_LINES );
	if( !tpl )
		CORAL_THROW( co::CoreException, "failed to load the template '" << templateName << "'" )

	expand( tpl, data );
}

void StreamEmitter::Emit( char c )
{
	_ostream << c;
}

void StreamEmitter::Emit( const std::string& s )
{
	_ostream << s;
}

void StreamEmitter::Emit( const char* s )
{
	_ostream << s;
}

void StreamEmitter::Emit( const char* s, size_t slen )
{
	_ostream.write( s, slen );
}

// ------ FileStreamEmitter --------------------------------------------------

FileStreamEmitter::FileStreamEmitter()
	: StreamEmitter( _ofstream )
{
	// empty
}

FileStreamEmitter::FileStreamEmitter( const std::string& dir, const std::string& filename )
	: StreamEmitter( _ofstream )
{
	reopen( dir, filename );
}

FileStreamEmitter::~FileStreamEmitter()
{
	_ofstream.close();
}

void FileStreamEmitter::reopen( const std::string& dir, const std::string& filename )
{
	if( _ofstream.is_open() )
		_ofstream.close();
	
	assert( !_ofstream.is_open() );

	bool pathCreated = co::Dir::makePath( dir.c_str() );
	if( !pathCreated )
		CORAL_THROW( co::CoreException, "failed to create directory '" << dir << "'" )

	std::string fullpath;
	fullpath.reserve( dir.length() + filename.length() + 2 );
	fullpath = dir;
	fullpath += '/';
	fullpath += filename;

	_ofstream.open( fullpath.c_str(), std::ios::out );
	if( !_ofstream.is_open() )
		CORAL_THROW( co::CoreException, "could not open file '" << fullpath << "' for writing" )
}
