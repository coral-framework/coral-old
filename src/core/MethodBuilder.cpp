/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "MethodBuilder.h"
#include "MethodInfo.h"
#include "TypeBuilder.h"
#include "ParameterInfo.h"
#include <co/Coral.h>
#include <co/System.h>
#include <co/ArrayRange.h>
#include <co/TypeManager.h>
#include <co/IllegalNameException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/NotSupportedException.h>
#include <co/reserved/LexicalUtils.h>
#include <sstream>

namespace co {

MethodBuilderComponent::MethodBuilderComponent() : _returnType( NULL )
{
	// empty
}

MethodBuilderComponent::~MethodBuilderComponent()
{
	// empty
}

void MethodBuilderComponent::init( TypeBuilder* typeBuilder, const std::string& name )
{
	assert( typeBuilder );

	_typeBuilder = typeBuilder;
	_name = name;
}

TypeBuilder* MethodBuilderComponent::getTypeBuilder()
{
	return _typeBuilder.get();
}

const std::string& MethodBuilderComponent::getMethodName()
{
	return _name;
}

void MethodBuilderComponent::defineReturnType( Type* type )
{
	if( _returnType )
		CORAL_THROW( NotSupportedException, "return type defined twice" );

	if( !type )
		CORAL_THROW( IllegalArgumentException, "illegal null return type" );

	_returnType = type;
}

void MethodBuilderComponent::defineParameter( const std::string& name, Type* type, bool input, bool output )
{
	if( !LexicalUtils::isValidIdentifier( name ) )
		CORAL_THROW( IllegalNameException, "parameter name '" << name << "' is not a valid identifier" );

	for( ArrayRange<ParameterInfo* const> r( _parameters ); r; r.popFirst() )
		if( r.getFirst()->getName() == name )
			CORAL_THROW( IllegalNameException, "parameter '" << name << "' defined twice in method '" << _name << "()'" );

	if( !type )
		CORAL_THROW( IllegalArgumentException, "illegal null parameter type" );

	TypeKind kind = type->getKind();
	if( kind == TK_EXCEPTION || kind == TK_COMPONENT )
		CORAL_THROW( IllegalArgumentException, "illegal parameter '" << name << "' - " <<
					( kind == TK_EXCEPTION ? "exceptions" : "components" ) << " cannot be passed as parameters" );

	if( !input && !output )
		CORAL_THROW( IllegalArgumentException, "parameter is neither input nor output" );

	ParameterInfoComponent* paramInfo = new ParameterInfoComponent();
	paramInfo->init( name, type, input, output );

	_parameters.push_back( paramInfo );
}

void MethodBuilderComponent::defineException( ExceptionType* exception )
{
	if( !exception )
		CORAL_THROW( IllegalArgumentException, "the passed exception is invalid" );

	_expectedExceptions.push_back( exception );
}

void MethodBuilderComponent::createMethod()
{
	assert( _typeBuilder.isValid() );

	MethodInfoComponent* mic = new MethodInfoComponent;
	_createdMethodInfo = mic;

	mic->setName( _name );
	mic->setParameters( _parameters );
	mic->setExceptions( _expectedExceptions );
	mic->setReturnType( _returnType );

	// notice: the call to addMethod() below may throw an exception if the TB's type was already created
	assert( dynamic_cast<TypeBuilderComponent*>( _typeBuilder.get() ) );
	static_cast<TypeBuilderComponent*>( _typeBuilder.get() )->addMethod( mic );
}

CORAL_EXPORT_COMPONENT( MethodBuilderComponent, MethodBuilderComponent );

} // namespace co
