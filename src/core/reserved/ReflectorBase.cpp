/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ReflectorBase.h"
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/InterfaceInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/UnsupportedOperationException.h>
#include <sstream>

namespace co {

// ------ ReflectorBase provides an interface named 'reflector', of type co::Reflector ------ //

InterfaceType* ReflectorBase_co_Reflector::getInterfaceType()
{
	return co::typeOf<Reflector>::get();
}

const std::string& ReflectorBase_co_Reflector::getInterfaceName()
{
	static const std::string s_interfaceName( "reflector" );
	return s_interfaceName;
}

// ------ ReflectorBase ------ //

ReflectorBase::ReflectorBase()
{
	// empty
}

ReflectorBase::~ReflectorBase()
{
	// empty
}

Component* ReflectorBase::getInterfaceOwner()
{
	return this;
}

void ReflectorBase::componentRetain()
{
	incrementRefCount();
}

void ReflectorBase::componentRelease()
{
	decrementRefCount();
}

ComponentType* ReflectorBase::getComponentType()
{
	return getOrCreateSimpleInternalComponentType( "co.ReflectorBase", "co.Reflector", "reflector" );
}

Interface* ReflectorBase::getInterface( InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );

	Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0: res = static_cast<Reflector*>( this ); break;
	default:
		raiseUnexpectedInterfaceIndex();
	}

	return res;
}

void ReflectorBase::bindInterface( InterfaceInfo* clientInterface, Interface* )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
}

void ReflectorBase::createValue( void*, size_t )
{
	raiseUnsupportedOperationException();
}

void ReflectorBase::copyValue( void*, void* )
{
	raiseUnsupportedOperationException();
}

void ReflectorBase::destroyValue( void* )
{
	raiseUnsupportedOperationException();
}

Component* ReflectorBase::newInstance()
{
	raiseUnsupportedOperationException();
	return NULL;
}

Interface* ReflectorBase::newProxy( DynamicProxyHandler* )
{
	raiseUnsupportedOperationException();
	return NULL;
}

void ReflectorBase::getAttribute( const Any&, AttributeInfo*, Any& )
{
	raiseUnsupportedOperationException();
}

void ReflectorBase::setAttribute( const Any&, AttributeInfo*, const Any& )
{
	raiseUnsupportedOperationException();
}

void ReflectorBase::invokeMethod( const Any&, MethodInfo*, ArrayRange<Any const>, Any& )
{
	raiseUnsupportedOperationException();
}

void ReflectorBase::checkValidSize( size_t expectedSize, size_t actualSize )
{
	if( expectedSize != actualSize )
		throw co::IllegalArgumentException( "instance size mismatch" );
}

void ReflectorBase::checValidProxyHandler( co::DynamicProxyHandler* handler )
{
	if( handler == NULL )
		throw co::IllegalArgumentException( "illegal null co::DynamicProxyHandler" );
}

void ReflectorBase::checkNumArguments( co::MethodInfo* mi, std::size_t numArgs )
{
	assert( mi );
	std::size_t expectedNumArgs = mi->getParameters().getSize();
	if( numArgs < expectedNumArgs )
		CORAL_THROW( co::MissingInputException, "method '" << mi->getName() << "' takes "
					<< expectedNumArgs << " argument" << ( expectedNumArgs > 1 ? "s" : "" )
					<< ", but only " << numArgs << ( numArgs > 1 ? " were" : " was" ) << " passed" );
}

void ReflectorBase::raiseAttributeIsReadOnly( co::AttributeInfo* ai )
{
	assert( ai && ai->getIsReadOnly() );
	CORAL_THROW( co::IllegalArgumentException, "attribute '" << ai->getName()
					<< "' is read-only and cannot be changed" );
}

void ReflectorBase::raiseArgumentTypeException( co::MethodInfo* mi, int argIndex, const co::IllegalCastException& e )
{
	CORAL_THROW( co::IllegalCastException, "invalid argument #" << argIndex + 1 << " to method "
					<< mi->getName() << "(): " << e.getMessage() );
}

void ReflectorBase::raiseUnexpectedMemberIndex()
{
	assert( false );
	throw co::IllegalArgumentException( "unexpected invalid member index" );
}

void ReflectorBase::raiseUnsupportedOperationException()
{
	throw co::UnsupportedOperationException( "operation not supported by this reflector" );
}


} // namespace co
