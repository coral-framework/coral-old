/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUACOMPONENT_H_
#define _LUACOMPONENT_H_

#include "Component_Base.h"
#include "LuaState.h"
#include <co/RefPtr.h>

namespace lua {

/*!
	A dynamic component, with interfaces defined in run-time and methods implemented in Lua.
 */
class LuaComponent : public lua::Component_Base
{
public:
    LuaComponent();
	virtual ~LuaComponent();

	// internal methods:
	void setComponentType( co::IComponentType* ct, int prototypeTableRef );
	void setComponentInstance( LuaComponent* prototype, int instanceTableRef );

	// co::IComponent methods:
	co::IComponentType* getComponentType();
	co::Interface* getInterface( co::IInterfaceInfo* itfInfo );
	void setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* instance );

	// co::IDynamicProxyHandler methods:
	co::int32 registerProxyInterface( co::Interface* proxy );
	const std::string& getProxyInterfaceName( co::int32 cookie );
	const co::Any& handleGetAttribute( co::int32 cookie, co::IAttributeInfo* ai );
	void handleSetAttribute( co::int32 cookie, co::IAttributeInfo* ai, const co::Any& value );
	const co::Any& handleMethodInvocation( co::int32 cookie, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args );

	// co::IReflector methods:
	co::int32 getSize();
	co::IType* getType();
	void createValue( void* address, size_t length );
    void copyValue( const void* fromAddress, void* toAddress );
    void destroyValue( void* address );
	co::IComponent* newInstance();
	co::Interface* newProxy( co::IDynamicProxyHandler* handler );
    void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value );
    void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value );
    void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& returnValue );
	void raise( const std::string& message );

private:
	inline const std::string& getInterfaceName( co::int32 cookie );
	void pushInterfaceInstanceTable( lua_State* L, co::int32 cookie );
	void pushAccessorName( lua_State* L, const char* prefix, const std::string& attribName );
	void getMethod( lua_State* L, int t, co::int32 cookie = -1 );

	co::Interface* getDynamicInterface( co::IInterfaceInfo* itfInfo );
	void bindToDynamicReceptacle( co::IInterfaceInfo* receptacle, co::Interface* instance );

	void raiseNotSupportedException();

private:
	// co.IComponentType that describes this Lua IComponent
	co::IComponentType* _componentType;

	/*
		Array of proxy interfaces created for the component's facets. If '_facets'
		is NULL, it means this is a 'component type', not a 'component instance'.
	 */
	co::Interface** _facets;

	// Length of the '_serverItfs' array.
	int _numFacets;

	/*
		Lua registry reference to the component's table.
		If this instance is a 'component type', '_tableRef' will be a reference to
		the component's prototype table. Otherwise, it will be a reference to
		the component's instance table.
	 */
	int _tableRef;

	// used by the co::IDynamicProxyHandler methods to return values
	co::Any _res;
};

} // namespace lua

#endif
