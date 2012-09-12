/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "TypeTraits.h"
#include "types/Type.h"
#include <co/Coral.h>
#include <co/IPort.h>
#include <co/IObject.h>
#include <co/IInterface.h>
#include <co/IComponent.h>
#include <co/NoSuchPortException.h>
#include <co/IllegalCastException.h>
#include <sstream>

namespace co {

const std::string TK_STRINGS[] =
{
	"null",
	"any",
	"bool",
	"int8",
	"uint8",
	"int16",
	"uint16",
	"int32",
	"uint32",
	"int64",
	"uint64",
	"float",
	"double",
	"string",
	"<array>",
	"<enum>",
	"<exception>",
	"<struct>",
	"<native class>",
	"<interface>",
	"<component>",
	"<off by one!>"
};

#define NEW_TYPE( k ) new TypeComponent( k )

IType* BASIC_TYPES[] =
{
	NEW_TYPE( TK_NULL ),
	NEW_TYPE( TK_ANY ),
	NEW_TYPE( TK_BOOL ),
	NEW_TYPE( TK_INT8 ),
	NEW_TYPE( TK_UINT8 ),
	NEW_TYPE( TK_INT16 ),
	NEW_TYPE( TK_UINT16 ),
	NEW_TYPE( TK_INT32 ),
	NEW_TYPE( TK_UINT32 ),
	NEW_TYPE( TK_INT64 ),
	NEW_TYPE( TK_UINT64 ),
	NEW_TYPE( TK_FLOAT ),
	NEW_TYPE( TK_DOUBLE ),
	NEW_TYPE( TK_STRING )
};

IType* getTypeByLiteralName( const char* fullName )
{
	return getType( fullName );
}

TypeKind getKind( IType* type )
{
	assert( type );
	return type->getKind();
}

IService* getServiceByType( IObject* object, IInterface* type )
{
	assert( object && type );
	IComponent* component = object->getComponent();
	Range<IPort*> facets = component->getFacets();
	for( ; facets; facets.popFirst() )
		if( facets.getFirst()->getType()->isA( type ) )
			return object->getServiceAt( facets.getFirst() );
	CORAL_THROW( NoSuchPortException, "component '" << component->getFullName()
		<< "' does not provide a facet of type '" << type->getFullName() << "'"  )
}

IService* getServiceByName( IObject* object, const std::string& portName )
{
	assert( object );
	IComponent* component = object->getComponent();
	IMember* port = component->getMember( portName );
	if( !port )
		CORAL_THROW( NoSuchPortException, "no such port '" << portName
			<< "' in component '" << component->getFullName() << "'" );
	return object->getServiceAt( static_cast<IPort*>( port ) );
}

void setServiceByName( IObject* object, const std::string& receptacleName, IService* service )
{
	assert( object );
	IComponent* component = object->getComponent();
	IMember* port = component->getMember( receptacleName );
	if( !port )
		CORAL_THROW( NoSuchPortException, "no such receptacle '" << receptacleName
			<< "' in component '" << component->getFullName() << "'" );
	object->setServiceAt( static_cast<IPort*>( port ), service );
}

bool isA( IService* service, IInterface* type )
{
	assert( type );
	return !service || service->getInterface()->isA( type );
}

void ensureIsA( IService* service, IInterface* type )
{
	assert( type );
	if( service && !service->getInterface()->isA( type ) )
		CORAL_THROW( IllegalCastException, "illegal cast from '"
				<< service->getInterface()->getFullName() << "' to '"
				<< type->getFullName() << "'" );
}

} // namespace co