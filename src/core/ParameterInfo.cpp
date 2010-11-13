/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ParameterInfo.h"
#include <co/IllegalNameException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/LexicalUtilities.h>
#include <sstream>

ParameterInfo::ParameterInfo() : _type( 0 ), _isIn( true ), _isOut( false )
{
	// empty
}

ParameterInfo::~ParameterInfo()
{
	// empty
}

void ParameterInfo::init( const std::string& name, co::Type* type, bool isIn, bool isOut )
{
	if( !co::LexicalUtilities::isValidIdentifier( name ) )
		CORAL_THROW( co::IllegalNameException, "the passed parameter name is invalid" );
	
	if( type == NULL )
		CORAL_THROW( co::IllegalArgumentException, "the passed parameter type is invalid" );

	if( isIn == false && isOut == false )
		CORAL_THROW( co::IllegalArgumentException, "both isIn and isOut are false" );

	setName( name );
	setType( type );
	setIsIn( isIn );
	setIsOut( isOut );
}

void ParameterInfo::setName( const std::string& name )
{
	_name = name;
}

void ParameterInfo::setType( co::Type* type )
{
	_type = type;
}

void ParameterInfo::setIsIn( bool isIn )
{
	_isIn = isIn;
}

void ParameterInfo::setIsOut( bool isOut )
{
	_isOut = isOut;
}

const std::string& ParameterInfo::getName()
{
	return _name;
}

co::Type* ParameterInfo::getType()
{
	return _type;
}

bool ParameterInfo::getIsIn()
{
	return _isIn;
}

bool ParameterInfo::getIsOut()
{
	return _isOut;
}

CORAL_EXPORT_COMPONENT( ParameterInfo, ParameterInfoComponent );
