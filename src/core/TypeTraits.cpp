/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeTraits.h"
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
	"<none>",
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

const std::string TK_STRINGS_CPP[] =
{
	"<NONE>",
	"co::Any",
	"bool",
	"co::int8",
	"co::uint8",
	"co::int16",
	"co::uint16",
	"co::int32",
	"co::uint32",
	"co::int64",
	"co::uint64",
	"float",
	"double",
	"std::string",
	"<ARRAY>",
	"<ENUM>",
	"<EXCEPTION>",
	"<STRUCT>",
	"<NATIVE_CLASS>",
	"<INTERFACE>",
	"<COMPONENT>",
	"<OFF_BY_ONE!>"
};

TypeKind getKind( IType* type )
{
	assert( type );
	return type->getKind();
}

IService* getServiceByType( IObject* object, IInterface* type )
{
	assert( object && type );
	IComponent* component = object->getComponent();
	Range<IPort* const> facets = component->getFacets();
	for( ; facets; facets.popFirst() )
		if( facets.getFirst()->getType()->isSubTypeOf( type ) )
			return object->getService( facets.getFirst() );
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
	return object->getService( static_cast<IPort*>( port ) );
}

void setServiceByName( IObject* object, const std::string& receptacleName, IService* service )
{
	assert( object );
	IComponent* component = object->getComponent();
	IMember* port = component->getMember( receptacleName );
	if( !port )
		CORAL_THROW( NoSuchPortException, "no such receptacle '" << receptacleName
			<< "' in component '" << component->getFullName() << "'" );
	object->setService( static_cast<IPort*>( port ), service );
}

CORAL_FORCE_INLINE bool _isA( IService* service, IInterface* type )
{
	assert( type );
	return !service || service->getInterface()->isSubTypeOf( type );
}

bool isA( IService* service, IInterface* type )
{
	return _isA( service, type );
}

void ensureIsA( IService* service, IInterface* type )
{
	if( !_isA( service, type ) )
		CORAL_THROW( IllegalCastException, "incompatible service ("
					<< type->getFullName() << " expected, got "
					<< service->getInterface()->getFullName() << ")" );
}

} // namespace co