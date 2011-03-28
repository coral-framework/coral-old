/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ReflectorBase.h"
#include <co/IMethod.h>
#include <co/IField.h>
#include <co/IPort.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/NotSupportedException.h>
#include <sstream>

namespace co {

// ------ ReflectorBase provides an interface named 'reflector', of type co::IReflector ------ //

IInterface* ReflectorBase_co_Reflector::getInterfaceType()
{
	return co::typeOf<IReflector>::get();
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

IObject* ReflectorBase::getInterfaceOwner()
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

IComponent* ReflectorBase::getComponentType()
{
	return getOrCreateInternalComponent( "co.ReflectorBase", "co.IReflector", "reflector" );
}

IService* ReflectorBase::getInterface( IPort* port )
{
	checkValidPort( port );

	IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0: res = static_cast<IReflector*>( this ); break;
	default:
		raiseUnexpectedPortIndex();
	}

	return res;
}

void ReflectorBase::setReceptacle( IPort* receptacle, IService* )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
}

void ReflectorBase::createValue( void*, size_t )
{
	raiseNotSupportedException();
}

void ReflectorBase::copyValue( const void*, void* )
{
	raiseNotSupportedException();
}

void ReflectorBase::destroyValue( void* )
{
	raiseNotSupportedException();
}

IObject* ReflectorBase::newInstance()
{
	raiseNotSupportedException();
	return NULL;
}

IService* ReflectorBase::newProxy( IDynamicServiceProvider* )
{
	raiseNotSupportedException();
	return NULL;
}

void ReflectorBase::getAttribute( const Any&, IField*, Any& )
{
	raiseNotSupportedException();
}

void ReflectorBase::setAttribute( const Any&, IField*, const Any& )
{
	raiseNotSupportedException();
}

void ReflectorBase::invokeMethod( const Any&, IMethod*, Range<Any const>, Any& )
{
	raiseNotSupportedException();
}

void ReflectorBase::raise( const std::string& )
{
	raiseNotSupportedException();
}

void ReflectorBase::checkValidSize( size_t expectedSize, size_t actualSize )
{
	if( expectedSize != actualSize )
		throw co::IllegalArgumentException( "instance size mismatch" );
}

void ReflectorBase::checkValidDynamicProvider( co::IDynamicServiceProvider* provider )
{
	if( provider == NULL )
		throw co::IllegalArgumentException( "illegal null co::IDynamicServiceProvider" );
}

void ReflectorBase::checkNumArguments( co::IMethod* mi, size_t numArgs )
{
	assert( mi );
	size_t expectedNumArgs = mi->getParameters().getSize();
	if( numArgs < expectedNumArgs )
		CORAL_THROW( co::MissingInputException, "method '" << mi->getName() << "' takes "
					<< expectedNumArgs << " argument" << ( expectedNumArgs > 1 ? "s" : "" )
					<< ", but only " << numArgs << ( numArgs > 1 ? " were" : " was" ) << " passed" );
}

void ReflectorBase::raiseAttributeIsReadOnly( co::IField* ai )
{
	assert( ai && ai->getIsReadOnly() );
	CORAL_THROW( co::IllegalArgumentException, "attribute '" << ai->getName()
					<< "' is read-only and cannot be changed" );
}

void ReflectorBase::raiseArgumentTypeException( co::IMethod* mi, int argIndex, const co::IllegalCastException& e )
{
	CORAL_THROW( co::IllegalCastException, "invalid argument #" << argIndex + 1 << " to method "
					<< mi->getName() << "(): " << e.getMessage() );
}

void ReflectorBase::raiseUnexpectedMemberIndex()
{
	assert( false );
	throw co::IllegalArgumentException( "unexpected invalid member index" );
}

void ReflectorBase::raiseNotSupportedException()
{
	throw co::NotSupportedException( "operation not supported by this reflector" );
}


} // namespace co
