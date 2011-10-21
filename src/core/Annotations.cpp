/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Annotations.h"

namespace co {

//---------- @co.CppBlock ------------------------------------------------------

CppBlockAnnotation::CppBlockAnnotation()
{
	// empty constructor
}

CppBlockAnnotation::~CppBlockAnnotation()
{
	// empty destructor
}

const std::string& CppBlockAnnotation::getValue()
{
	return _value;
}

void CppBlockAnnotation::setValue( const std::string& value )
{
	_value = value;
}

CORAL_EXPORT_COMPONENT( CppBlockAnnotation, CppBlockAnnotation );

//---------- @co.Documentation -------------------------------------------------

DocumentationAnnotation::DocumentationAnnotation()
{
	// empty constructor
}

DocumentationAnnotation::~DocumentationAnnotation()
{
	// empty destructor
}

const std::string& DocumentationAnnotation::getValue()
{
	return _value;
}

void DocumentationAnnotation::setValue( const std::string& value )
{
	_value = value;
}

void DocumentationAnnotation::addDocFor( const std::string& element, const std::string& text )
{
	DocMap::iterator it = _docMap.find( element );
	if( it != _docMap.end() )
	{
		std::string& str = it->second;
		str.reserve( str.length() + 1 + text.length() );
		str.push_back( '\n' );
		str.append( text );
	}
	else
	{
		_docMap.insert( DocMap::value_type( element, text ) );
	}
}

const std::string& DocumentationAnnotation::getDocFor( const std::string& element )
{
	static const std::string s_emptyString;

	DocMap::iterator it = _docMap.find( element );
	if( it == _docMap.end() )
		return s_emptyString;

	return it->second;
}

CORAL_EXPORT_COMPONENT( DocumentationAnnotation, DocumentationAnnotation );

} // namespace co
