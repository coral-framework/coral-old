/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ParameterInfo.h"
#include <co/IllegalNameException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/LexicalUtils.h>
#include <sstream>

namespace co {

ParameterInfoComponent::ParameterInfoComponent() : _type( 0 ), _isIn( true ), _isOut( false )
{
	// empty
}

ParameterInfoComponent::~ParameterInfoComponent()
{
	// empty
}

void ParameterInfoComponent::init( const std::string& name, Type* type, bool isIn, bool isOut )
{
	if( !LexicalUtils::isValidIdentifier( name ) )
		CORAL_THROW( IllegalNameException, "the passed parameter name is invalid" );
	
	if( type == NULL )
		CORAL_THROW( IllegalArgumentException, "the passed parameter type is invalid" );

	if( isIn == false && isOut == false )
		CORAL_THROW( IllegalArgumentException, "both isIn and isOut are false" );

	setName( name );
	setType( type );
	setIsIn( isIn );
	setIsOut( isOut );
}

void ParameterInfoComponent::setName( const std::string& name )
{
	_name = name;
}

void ParameterInfoComponent::setType( Type* type )
{
	_type = type;
}

void ParameterInfoComponent::setIsIn( bool isIn )
{
	_isIn = isIn;
}

void ParameterInfoComponent::setIsOut( bool isOut )
{
	_isOut = isOut;
}

const std::string& ParameterInfoComponent::getName()
{
	return _name;
}

Type* ParameterInfoComponent::getType()
{
	return _type;
}

bool ParameterInfoComponent::getIsIn()
{
	return _isIn;
}

bool ParameterInfoComponent::getIsOut()
{
	return _isOut;
}

CORAL_EXPORT_COMPONENT( ParameterInfoComponent, ParameterInfoComponent );

} // namespace co
