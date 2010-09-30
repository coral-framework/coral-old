/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "LuaComponent.h"
#include "LuaState.h"
#include <co/InterfaceInfo.h>
#include <co/UnsupportedOperationException.h>
#include <lua/Exception.h>
#include <cassert>

LuaComponent::LuaComponent()
{
	_customType = NULL;
	_serverItfs = NULL;
	_numServerItfs = 0;
	_tableRef = LUA_NOREF;
}

LuaComponent::~LuaComponent()
{
	luaL_unref( LuaState::getL(), LUA_REGISTRYINDEX, _tableRef );
	for( int i = 0; i < _numServerItfs; ++i )
		delete _serverItfs[i];
	delete[] _serverItfs;
}

void LuaComponent::setComponentType( co::ComponentType* ct, int prototypeTableRef )
{
	assert( ct != NULL && _customType == NULL && _serverItfs == NULL );

	_customType = ct;
	_tableRef = prototypeTableRef;

	// register ourself as the type's reflector
	_customType->setReflector( this );
}

void LuaComponent::setComponentInstance( int instanceTableRef )
{
	assert( _tableRef == LUA_NOREF && _serverItfs != NULL );
	_tableRef = instanceTableRef;
}

co::ComponentType* LuaComponent::getComponentType()
{
	return _customType ? _customType : lua::Component_Base::getComponentType();
}

co::Interface* LuaComponent::getInterface( co::InterfaceInfo* itfInfo )
{
	co::CompoundType* owner = itfInfo->getOwner();
	if( owner == _customType )
		return getCustomInterface( itfInfo );
	return lua::Component_Base::getInterface( itfInfo );
}

void LuaComponent::bindInterface( co::InterfaceInfo* clientItfInfo, co::Interface* instance )
{
	co::CompoundType* owner = clientItfInfo->getOwner();
	if( owner == _customType )
		bindCustomInterface( clientItfInfo, instance );
	else
		lua::Component_Base::bindInterface( clientItfInfo, instance );
}

co::int32 LuaComponent::registerProxyInterface( co::Interface* proxy )
{
	_serverItfs[_numServerItfs] = proxy;
	return _numServerItfs++;
}

const std::string& LuaComponent::getProxyInterfaceName( co::int32 cookie )
{
	return _customType->getProvidedInterfaces()[cookie]->getName();
}

void LuaComponent::handleGetAttribute( co::int32 cookie, co::AttributeInfo* ai, co::Any& value )
{
	// TODO
}

void LuaComponent::handleSetAttribute( co::int32 cookie, co::AttributeInfo* ai, const co::Any& value )
{
	// TODO
}

void LuaComponent::handleMethodInvocation( co::int32 cookie, co::MethodInfo* mi,
											co::ArrayRange<co::Any const> args, co::Any& returnValue )
{
	// TODO
}

co::int32 LuaComponent::getSize()
{
	return sizeof(LuaComponent);
}

co::Type* LuaComponent::getType()
{
	return _customType;
}

void LuaComponent::createValue( void*, size_t )
{
	raiseUnsupportedOperationException();
}

void LuaComponent::copyValue( const void*, void* )
{
	raiseUnsupportedOperationException();
}

void LuaComponent::destroyValue( void* )
{
	raiseUnsupportedOperationException();
}

co::Component* LuaComponent::newInstance()
{
	LuaComponent* newLC = new LuaComponent;
	newLC->_customType = _customType;

	// create our server proxy interfaces
	co::ArrayRange<co::InterfaceInfo* const> serverItfs = _customType->getProvidedInterfaces();
	int numServerItfs = static_cast<int>( serverItfs.getSize() );

	newLC->_serverItfs = new co::Interface*[numServerItfs];
	for( int i = 0; i < numServerItfs; ++i )
	{
		/*
			To avoid having exceptions raised here by getReflector(), we
			should check all interface reflectors before creating the Component.
		 */
		assert( newLC->_numServerItfs == i );
		serverItfs[i]->getType()->getReflector()->newProxy( newLC );
		assert( newLC->_numServerItfs == i + 1 );
	}

	return newLC;
}

co::Interface* LuaComponent::newProxy( co::DynamicProxyHandler* )
{
	raiseUnsupportedOperationException();
	return NULL;
}

void LuaComponent::getAttribute( const co::Any&, co::AttributeInfo*, co::Any& )
{
	raiseUnsupportedOperationException();
}

void LuaComponent::setAttribute( const co::Any&, co::AttributeInfo*, const co::Any& )
{
	raiseUnsupportedOperationException();
}

void LuaComponent::invokeMethod( const co::Any&, co::MethodInfo*, co::ArrayRange<co::Any const>, co::Any& )
{
	raiseUnsupportedOperationException();
}

co::Interface* LuaComponent::getCustomInterface( co::InterfaceInfo* itfInfo )
{
	if( itfInfo->getIsProvided() )
	{
		int idx = itfInfo->getIndex() - static_cast<int>( _customType->getRequiredInterfaces().getSize() );
		return _serverItfs[idx];
	}

	return NULL;
}

void LuaComponent::bindCustomInterface( co::InterfaceInfo* clientItfInfo, co::Interface* instance )
{
	// TODO
}

void LuaComponent::raiseUnsupportedOperationException()
{
	throw co::UnsupportedOperationException( "operation not supported by this reflector" );
}

CORAL_EXPORT_COMPONENT( LuaComponent, Component );
