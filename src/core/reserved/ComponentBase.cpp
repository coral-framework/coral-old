/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ComponentBase.h"
#include "../BasicReflector.h"
#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/System.h>
#include <co/Namespace.h>
#include <co/TypeBuilder.h>
#include <co/TypeManager.h>
#include <co/CompoundType.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>
#include <co/TypeCreationTransaction.h>
#include <co/NoSuchInterfaceException.h>
#include <sstream>
#include <iostream>

namespace co {

ComponentBase::~ComponentBase()
{
	if( _refCount != 0 )
	{
		// TODO call a handler function instead
		std::cerr << "Deleting component " << this <<
			" with a reference count of " << _refCount << ". Memory corruption possible.";

		assert( false );
    }
}

InterfaceType* ComponentBase::getInterfaceType()
{
	return typeOf<Component>::get();
}

const std::string& ComponentBase::getInterfaceName()
{
	static const std::string s_interfaceName( "component" );
	return s_interfaceName;
}

void ComponentBase::checkValidInterface( InterfaceInfo* itfInfo )
{
	if( !itfInfo )
		throw NoSuchInterfaceException( "illegal null interface info" );

	ComponentType* myType = getComponentType();
	CompoundType* owner = itfInfo->getOwner();
	if( owner != static_cast<CompoundType*>( myType ) )
		CORAL_THROW( NoSuchInterfaceException, "interface '" << itfInfo->getName() << "' belongs to "
			<< owner->getFullName() << ", not to " << myType->getFullName() );

	assert( itfInfo->getIndex() < myType->getInterfaces().getSize() );
}

void ComponentBase::checkValidClientInterface( InterfaceInfo* itfInfo )
{
	checkValidInterface( itfInfo );
	if( itfInfo->getIsProvided() )
		throw NoSuchInterfaceException( "expected a client interface, but got a server interface" );
}

void ComponentBase::raiseUnexpectedInterfaceIndex()
{
	assert( false );
	throw NoSuchInterfaceException( "unexpected invalid interface index" );
}

void ComponentBase::raiseIncompatibleInterface( InterfaceType* expectedType, Interface* ptr )
{
	CORAL_THROW( IllegalArgumentException, "expected a " << expectedType->getFullName()
					<< ", but got a " << ptr->getInterfaceType()->getFullName() );
}
	
ComponentType* ComponentBase::getOrCreateSimpleInternalComponentType( const char* componentTypeName, 
																	  const char* interfaceTypeName,
																	  const char* interfaceName )
{
	std::string fullTypeName( componentTypeName );

	// get the ComponentType if it already exists
	TypeManager* tm = getSystem()->getTypes();
	Type* type = tm->findType( fullTypeName );
	if( type )
	{
		assert( dynamic_cast<ComponentType*>( type ) );
		return dynamic_cast<ComponentType*>( type );
	}

	// create the ComponentType if it's not defined
	InterfaceType* interfaceType = dynamic_cast<InterfaceType*>( getType( interfaceTypeName ) );
	assert( interfaceType );

	RefPtr<TypeCreationTransaction> tct =
			newInstance( "co.TypeCreationTransactionComponent" )->getProvided<TypeCreationTransaction>();

	std::size_t lastDotPos = fullTypeName.rfind( '.' );
	assert( lastDotPos != std::string::npos ); // componentTypeName must be specified with a namespace

	std::string namespaceName( fullTypeName.substr( 0, lastDotPos ) );
	std::string localTypeName( fullTypeName.substr( lastDotPos + 1 ) );

	Namespace* ns = tm->findNamespace( namespaceName );
	assert( ns ); // the namespace should have been created before

	RefPtr<TypeBuilder> tb = ns->defineType( localTypeName, TK_COMPONENT, tct.get() );
	tb->defineInterface( interfaceName, interfaceType, true );

	try
	{
		tct->commit();
	}
	catch( std::exception& )
	{
		tct->rollback();
		throw;
	}

	ComponentType* componentType = dynamic_cast<ComponentType*>( tb->createType() );
	assert( componentType );

	// set the ComponentType with a dummy reflector
	componentType->setReflector( new BasicReflector( componentType ) );

	return componentType;
}

} // namespace 'co'
