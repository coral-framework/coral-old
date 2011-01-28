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

MethodBuilder::MethodBuilder() : _returnType( NULL )
{
	// empty
}

MethodBuilder::~MethodBuilder()
{
	// empty
}

void MethodBuilder::init( co::TypeBuilder* typeBuilder, const std::string& name )
{
	assert( typeBuilder );

	_typeBuilder = typeBuilder;
	_name = name;
}

co::TypeBuilder* MethodBuilder::getTypeBuilder()
{
	return _typeBuilder.get();
}

const std::string& MethodBuilder::getMethodName()
{
	return _name;
}

void MethodBuilder::defineReturnType( co::Type* type )
{
	if( _returnType )
		CORAL_THROW( co::NotSupportedException, "return type defined twice" );

	if( !type )
		CORAL_THROW( co::IllegalArgumentException, "illegal null return type" );

	_returnType = type;
}

void MethodBuilder::defineParameter( const std::string& name, co::Type* type, bool input, bool output )
{
	if( !co::LexicalUtils::isValidIdentifier( name ) )
		CORAL_THROW( co::IllegalNameException, "parameter name '" << name << "' is not a valid identifier" );

	for( co::ArrayRange<co::ParameterInfo* const> r( _parameters ); r; r.popFirst() )
		if( r.getFirst()->getName() == name )
			CORAL_THROW( co::IllegalNameException, "parameter '" << name << "' defined twice in method '" << _name << "()'" );

	if( !type )
		CORAL_THROW( co::IllegalArgumentException, "illegal null parameter type" );

	co::TypeKind kind = type->getKind();
	if( kind == co::TK_EXCEPTION || kind == co::TK_COMPONENT )
		CORAL_THROW( co::IllegalArgumentException, "illegal parameter '" << name << "' - " <<
					( kind == co::TK_EXCEPTION ? "exceptions" : "components" ) << " cannot be passed as parameters" );

	if( !input && !output )
		CORAL_THROW( co::IllegalArgumentException, "parameter is neither input nor output" );

	ParameterInfo* paramInfo = new ParameterInfo();
	paramInfo->init( name, type, input, output );

	_parameters.push_back( paramInfo );
}

void MethodBuilder::defineException( co::ExceptionType* exception )
{
	if( !exception )
		CORAL_THROW( co::IllegalArgumentException, "the passed exception is invalid" );

	_expectedExceptions.push_back( exception );
}

void MethodBuilder::createMethod()
{
	assert( _typeBuilder.isValid() );

	co::RefPtr<MethodInfo> mi = new MethodInfo;
	mi->setName( _name );
	mi->setParameters( _parameters );
	mi->setExceptions( _expectedExceptions );
	mi->setReturnType( _returnType );

	_createdMethodInfo = mi.get();

	// notice: the call to addMethod() below may throw an exception if the TB's type was already created
	static_cast<TypeBuilder*>( _typeBuilder.get() )->addMethod( _createdMethodInfo.get() );
}

CORAL_EXPORT_COMPONENT( MethodBuilder, MethodBuilderComponent );
