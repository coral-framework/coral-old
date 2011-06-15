/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Type.h"
#include "BasicReflector.h"
#include "SignatureCalculator.h"
#include <co/Coral.h>
#include <co/ISystem.h>
#include <co/INamespace.h>
#include <co/IModuleManager.h>
#include <co/ModuleLoadException.h>
#include <cassert>
#include <sstream>

namespace co {

// ------ TypeImpl -------------------------------------------------------------

TypeImpl::TypeImpl() : _namespace( NULL ), _kind( TK_NONE )
{
	_hasSignatures = false;
	_isCalculatingSignatures = false;
}

void TypeImpl::setType( INamespace* parent, const std::string& name, TypeKind kind )
{
	assert( _kind == TK_NONE );
	assert( _namespace == NULL );

	_namespace = parent;
	_name = name;
	_kind = kind;

	// compute our 'fullName'
	if( _namespace->getParentNamespace() )
	{
		// we're not in the global namespace
		const std::string& namespaceFullName = _namespace->getFullName();
		_fullName.reserve( namespaceFullName.length() + name.length() + 1 );
		_fullName = namespaceFullName;
		_fullName.push_back( '.' );
	}

	_fullName.append( name );
}

const std::string& TypeImpl::getName()
{
	return _name;
}

const std::string& TypeImpl::getFullName()
{
	return _fullName;
}

INamespace* TypeImpl::getNamespace()
{
	return _namespace;
}

TypeKind TypeImpl::getKind()
{
	return _kind;
}

const Uuid& TypeImpl::getFullSignature( IType* myType )
{
	if( !_hasSignatures )
		calculateSignatures( myType );

	return _fullSignature;
}

const Uuid& TypeImpl::getBinarySignature( IType* myType )
{
	if( !_hasSignatures )
		calculateSignatures( myType );

	return _binarySignature;
}

IReflector* TypeImpl::getReflector( IType* myType )
{
	if( _reflector.isValid() )
		return _reflector.get();

	if( _kind < TK_STRUCT )
	{
		// basic reflectors are instantiated on demand
		_reflector = BasicReflector::create( myType );
	}
	else
	{
		/*
			Loading the type's module should cause its reflector to be installed.
			Notice that reflectors cannot be obtained before the system is set up
			(ISystem::setupBase()), since we cannot load modules before that.
		 */
		try
		{
			getSystem()->getModules()->load( myType->getNamespace()->getFullName() );
		}
		catch( std::exception& e )
		{
			// if an exception was raised, it is fair to expect the reflector was not installed
			assert( !_reflector.isValid() );

			CORAL_THROW( ModuleLoadException, "could not obtain a reflector for '"
							<< myType->getFullName() << "': " << e.what() );
		}
		catch( ... )
		{
			CORAL_THROW( ModuleLoadException, "could not obtain a reflector for '"
							<< myType->getFullName() << "': unknown exception" );
		}
	}

	if( _reflector.isValid() )
		return _reflector.get();

	CORAL_THROW( ModuleLoadException, "type '" << myType->getFullName() << "' has no reflector" );
}

void TypeImpl::setReflector( IReflector* reflector )
{
	_reflector = reflector;
}

void TypeImpl::calculateSignatures( IType* myType )
{
	assert( !_isCalculatingSignatures );
	_isCalculatingSignatures = true;

	SignatureCalculator sc( myType );
	sc.calculate();

	_fullSignature = sc.getFullSignature();
	_binarySignature = sc.getBinarySignature();

	_hasSignatures = true;
	_isCalculatingSignatures = false;
}

// ------ IType -----------------------------------------------------------------

Type::~Type()
{
	// empty
}

CORAL_EXPORT_COMPONENT( Type, Type );

} // namespace co
