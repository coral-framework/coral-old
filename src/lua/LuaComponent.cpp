/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "LuaComponent.h"
#include "LuaBinding.h"
#include <co/MethodInfo.h>
#include <co/ParameterInfo.h>
#include <co/AttributeInfo.h>
#include <co/InterfaceInfo.h>
#include <co/UnsupportedOperationException.h>
#include <lua/Exception.h>
#include <cctype>
#include <cassert>
#include <sstream>

/*
	Helper macros to get a lua_State* L and protect
	the Lua stack from eventual exceptions.
 */
#define __BEGIN_LUA_API_CODE__ \
	lua_State* L = LuaState::getL(); \
	int originalTop = lua_gettop( L ); \
	try {

#define __END_LUA_API_CODE__ \
	} catch( ... ) { lua_settop( L, originalTop ); throw; } \
	lua_settop( L, originalTop );


/*****************************************************************************/
/*  LuaComponent                                                             */
/*****************************************************************************/

LuaComponent::LuaComponent()
{
	_componentType = NULL;
	_facets = NULL;
	_numFacets = 0;
	_tableRef = LUA_NOREF;
}

LuaComponent::~LuaComponent()
{
	lua_State* L = LuaState::getL();
	if( L )
		luaL_unref( L, LUA_REGISTRYINDEX, _tableRef );
	for( int i = 0; i < _numFacets; ++i )
		delete _facets[i];
	delete[] _facets;
}

void LuaComponent::setComponentType( co::ComponentType* ct, int prototypeTableRef )
{
	assert( ct != NULL && _componentType == NULL && _facets == NULL );

	_componentType = ct;
	_tableRef = prototypeTableRef;

	// register ourself as the type's reflector
	_componentType->setReflector( this );
}

void LuaComponent::setComponentInstance( LuaComponent* prototype, int instanceTableRef )
{
	assert( prototype && prototype->_facets == NULL );

	_componentType = prototype->_componentType;
	_tableRef = instanceTableRef;

	// create proxy interfaces for our facets
	co::ArrayRange<co::InterfaceInfo* const> facets = _componentType->getFacets();
	int numFacets = static_cast<int>( facets.getSize() );
	_facets = new co::Interface*[numFacets];
	for( int i = 0; i < numFacets; ++i )
	{
		/*
			To avoid having exceptions raised here by getReflector(), we
			should check all interface reflectors before creating the Component.
		 */
		assert( _numFacets == i );
		facets[i]->getType()->getReflector()->newProxy( this );
		assert( _numFacets == i + 1 );
	}
}

co::ComponentType* LuaComponent::getComponentType()
{
	return _componentType ? _componentType : lua::Component_Base::getComponentType();
}

co::Interface* LuaComponent::getInterface( co::InterfaceInfo* itfInfo )
{
	co::CompoundType* owner = itfInfo->getOwner();
	if( owner == _componentType )
		return getDynamicInterface( itfInfo );
	return lua::Component_Base::getInterface( itfInfo );
}

void LuaComponent::bindInterface( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	co::CompoundType* owner = receptacle->getOwner();
	if( owner == _componentType )
		bindToDynamicReceptacle( receptacle, instance );
	else
		lua::Component_Base::bindInterface( receptacle, instance );
}

co::int32 LuaComponent::registerProxyInterface( co::Interface* proxy )
{
	_facets[_numFacets] = proxy;
	return _numFacets++;
}

const std::string& LuaComponent::getInterfaceName( co::int32 cookie )
{
	return _componentType->getFacets()[cookie]->getName();
}

const std::string& LuaComponent::getProxyInterfaceName( co::int32 cookie )
{
	return getInterfaceName( cookie );
}

void LuaComponent::pushInterfaceInstanceTable( lua_State* L, co::int32 cookie )
{
	lua_rawgeti( L, LUA_REGISTRYINDEX, _tableRef );
	lua_pushstring( L, getInterfaceName( cookie ).c_str() );
	lua_rawget( L, -2 );
	lua_remove( L, -2 );
}

void LuaComponent::pushAccessorName( lua_State* L, const char* prefix, const std::string& attribName )
{
	char buffer[256];
	char* p = buffer;
	while( *prefix )
		*p++ = *prefix++;
	assert( !attribName.empty() && attribName.length() < sizeof(buffer) - ( p - buffer ) );
	const char* str = attribName.c_str();
	*p++ = toupper( *str++ );
	while( *str )
		*p++ = *str++;
	lua_pushlstring( L, buffer, p - buffer );
}

void LuaComponent::getMethod( lua_State* L, int t, co::int32 cookie )
{
	const char* methodName = lua_tostring( L, -1 );
	lua_gettable( L, t );
	if( lua_isnil( L, -1 ) )
	{
		std::stringstream ss;
		ss << "missing method '" << methodName << "' in LuaComponent '" << _componentType->getFullName() << "'";
		if( cookie != -1 )
			ss << ", interface '" << getInterfaceName( cookie ) << "'";
		throw lua::Exception( ss.str() );
	}
}

const co::Any& LuaComponent::handleGetAttribute( co::int32 cookie, co::AttributeInfo* ai )
{
	__BEGIN_LUA_API_CODE__

	pushInterfaceInstanceTable( L, cookie );
	pushAccessorName( L, "get", ai->getName() );
	getMethod( L, -2, cookie );
	lua_pushvalue( L, -2 ); // push the 'self' argument
	LuaState::call( L, 1, 1 );
	LuaState::getAny( L, -1, ai->getType(), _res );

	__END_LUA_API_CODE__

	return _res;
}

void LuaComponent::handleSetAttribute( co::int32 cookie, co::AttributeInfo* ai, const co::Any& value )
{
	__BEGIN_LUA_API_CODE__

	pushInterfaceInstanceTable( L, cookie );
	pushAccessorName( L, "set", ai->getName() );
	getMethod( L, -2, cookie );
	lua_pushvalue( L, -2 ); // push the 'self' argument
	LuaState::push( L, value );
	LuaState::call( L, 2, 0 );

	__END_LUA_API_CODE__
}

const co::Any& LuaComponent::handleMethodInvocation( co::int32 cookie, co::MethodInfo* mi, co::ArrayRange<co::Any const> args )
{
	__BEGIN_LUA_API_CODE__

	pushInterfaceInstanceTable( L, cookie );
	lua_pushstring( L, mi->getName().c_str() );
	getMethod( L, -2, cookie );
	lua_pushvalue( L, -2 ); // push the 'self' argument

	co::Type* returnType = mi->getReturnType();
	int numOut = returnType ? 1 : 0;
	int numIn = 1;

	co::ArrayRange<co::ParameterInfo* const> paramList = mi->getParameters();
	assert( paramList.getSize() == args.getSize() );

	int numArgs = static_cast<int>( args.getSize() );
	for( int i = 0; i < numArgs; ++i )
	{
		co::ParameterInfo* param = paramList[i];
		if( param->getIsIn() )
		{
			LuaState::push( L, args[i] );
			++numIn;
		}
		if( param->getIsOut() )
			++numOut;
	}

	LuaState::call( L, numIn, numOut );

	// get the function result, if needed
	int idx = lua_gettop( L ) + 1 - numOut;
	if( returnType )
	{
		try
		{
			LuaState::getAny( L, idx++, returnType, _res );
		}
		catch( std::exception& e )
		{
			CORAL_THROW( lua::Exception, "bad result for method '" << mi->getName() << "' (" << e.what() << ")" );
		}
	}
	else
	{
		_res.clear();
	}

	// get other output values
	int i = 0;
	try
	{
		for( ; i < numArgs; ++i )
		{
			if( paramList[i]->getIsOut() )
				LuaState::getValue( L, idx++, args[i] );
		}
	}
	catch( std::exception& e )
	{
		CORAL_THROW( lua::Exception, "bad result for parameter #" << i + 1 << " in method '"
						<< mi->getName() << "' (" << e.what() << ")" );
	}

	__END_LUA_API_CODE__

	return _res;
}

co::int32 LuaComponent::getSize()
{
	return sizeof(LuaComponent);
}

co::Type* LuaComponent::getType()
{
	return _componentType;
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
	assert( _facets == NULL && _tableRef != LUA_NOREF );

	co::Any any;

	__BEGIN_LUA_API_CODE__

	// call the component prototype table to produce a new instance
	lua_rawgeti( L, LUA_REGISTRYINDEX, _tableRef );
	lua_getmetatable( L, -1 );
	lua_pushliteral( L, "__call" );
	lua_rawget( L, -2 ); // get the ComponentMT.__call function
	lua_pushvalue( L, -3 ); // pass the component's proto table as argument

	LuaState::call( L, 1, 1 );

	// extract the returned co::Component
	CompoundTypeBinding::getInstance( L, -1, any );

	__END_LUA_API_CODE__

	return any.get<co::Component*>();
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

co::Interface* LuaComponent::getDynamicInterface( co::InterfaceInfo* itfInfo )
{
	if( itfInfo->getIsFacet() )
		return _facets[itfInfo->getIndex()];

	// call the Lua component to get a client interface
	co::Interface* res;
	__BEGIN_LUA_API_CODE__

	lua_rawgeti( L, LUA_REGISTRYINDEX, _tableRef );
	pushAccessorName( L, "getReceptacle", itfInfo->getName() );
	getMethod( L, -2 );
	lua_pushvalue( L, -2 ); // push the 'self' argument
	LuaState::call( L, 1, 1 );

	co::Any any;
	any.set<co::Interface*&>( res );
	LuaState::getValue( L, -1, any );

	__END_LUA_API_CODE__

	// check interface compatibility
	if( res && !res->getInterfaceType()->isSubTypeOf( itfInfo->getType() ) )
		raiseIncompatibleInterface( itfInfo->getType(), res );

	return res;
}

void LuaComponent::bindToDynamicReceptacle( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	// check interface compatibility
	if( instance && !instance->getInterfaceType()->isSubTypeOf( receptacle->getType() ) )
		raiseIncompatibleInterface( receptacle->getType(), instance );

	__BEGIN_LUA_API_CODE__

	lua_rawgeti( L, LUA_REGISTRYINDEX, _tableRef );
	pushAccessorName( L, "setReceptacle", receptacle->getName() );
	getMethod( L, -2 );
	lua_pushvalue( L, -2 ); // push the 'self' argument
	LuaState::push( L, instance );
	LuaState::call( L, 2, 0 );

	__END_LUA_API_CODE__
}

void LuaComponent::raiseUnsupportedOperationException()
{
	throw co::UnsupportedOperationException( "operation not supported by this reflector" );
}

CORAL_EXPORT_COMPONENT( LuaComponent, Component );
