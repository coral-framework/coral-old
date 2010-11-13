/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "Type.h"
#include "BasicReflector.h"
#include "SignatureCalculator.h"
#include <co/Coral.h>
#include <co/System.h>
#include <co/Namespace.h>
#include <co/ModuleManager.h>
#include <co/ModuleLoadException.h>
#include <cassert>
#include <sstream>

// --- TypeImpl ---

TypeImpl::TypeImpl() : _namespace( NULL ), _kind( co::TK_NONE )
{
	_hasSignatures = false;
	_isCalculatingSignatures = false;
}

void TypeImpl::setType( co::Namespace* parent, const std::string& name, co::TypeKind kind )
{
	assert( _kind == co::TK_NONE );
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

co::Namespace* TypeImpl::getNamespace()
{
	return _namespace;
}

co::TypeKind TypeImpl::getKind()
{
	return _kind;
}

const co::Uuid& TypeImpl::getFullSignature( co::Type* myType )
{
	if( !_hasSignatures )
		calculateSignatures( myType );

	return _fullSignature;
}

const co::Uuid& TypeImpl::getBinarySignature( co::Type* myType )
{
	if( !_hasSignatures )
		calculateSignatures( myType );

	return _binarySignature;
}

co::Reflector* TypeImpl::getReflector( co::Type* myType )
{
	if( !_reflector.isValid() )
	{
		if( _kind < co::TK_STRUCT )
		{
			// BasicReflectors are instantiated on demand
			_reflector = new BasicReflector( myType );
		}
		else
		{
			/*
				Loading the type's module should cause its reflector to be installed.
				Notice that reflectors cannot be obtained before the system is set up
				(co::System::setupBase()), since we cannot load modules before that.
			 */
			try
			{
				co::getSystem()->getModules()->load( myType->getNamespace()->getFullName() );
			}
			catch( std::exception& e )
			{
				// if an exception was raised, it is fair to expect the reflector was not installed
				assert( !_reflector.isValid() );

				CORAL_THROW( co::ModuleLoadException, "could not obtain a reflector for '"
								<< myType->getFullName() << "': " << e.what() );
			}
			catch( ... )
			{
				CORAL_THROW( co::ModuleLoadException, "could not obtain a reflector for '"
								<< myType->getFullName() << "': unknown exception" );
			}
		}
	}

	if( !_reflector.isValid() )
		CORAL_THROW( co::ModuleLoadException, "type '" << myType->getFullName() << "' has no reflector" );

	return _reflector.get();
}

void TypeImpl::setReflector( co::Reflector* reflector )
{
	_reflector = reflector;
}

void TypeImpl::calculateSignatures( co::Type* myType )
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

// --- Type ---

Type::~Type()
{
	// empty
}

CORAL_EXPORT_COMPONENT( Type, TypeComponent );
