/*
 * Coral - Lightweight C++ Component Framework
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
	void setComponent( co::IComponent* ct, int prototypeTableRef );
	void releaseComponent();

	void setInstance( LuaComponent* prototype, int instanceTableRef );

	// co::IObject methods:
	co::IComponent* getComponent();
	co::IService* getServiceAt( co::IPort* port );
	void setServiceAt( co::IPort* receptacle, co::IService* instance );

	// co::IDynamicServiceProvider methods:
	co::int32 dynamicRegisterService( co::IService* proxy );
	co::IPort* dynamicGetFacet( co::int32 cookie );
	const co::Any& dynamicGetField( co::int32 cookie, co::IField* ai );
	void dynamicSetField( co::int32 cookie, co::IField* ai, const co::Any& value );
	const co::Any& dynamicInvoke( co::int32 cookie, co::IMethod* mi, co::Range<co::Any const> args );

	// co::IReflector methods:
	co::uint32 getSize();
	co::IType* getType();
	void createValue( void* address );
    void copyValue( const void* fromAddress, void* toAddress );
    void destroyValue( void* address );
	co::IObject* newInstance();
	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider );
    void getField( const co::Any& instance, co::IField* ai, co::Any& value );
    void setField( const co::Any& instance, co::IField* ai, const co::Any& value );
    void invoke( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& returnValue );
	void raise( const std::string& message );

private:
	inline co::IPort* getFacetByCookie( co::int32 cookie );
	void pushFacetTable( lua_State* L, co::int32 cookie );
	void pushAccessorName( lua_State* L, const char* prefix,
			const std::string& fieldName, const char* suffix = NULL );
	void getMethod( lua_State* L, int t, co::int32 cookie = -1 );

	co::IService* dynamicGetService( co::IPort* port );
	void dynamicSetService( co::IPort* receptacle, co::IService* instance );

	void raiseNotSupportedException();

private:
	// co.IComponent that describes this Lua Component
	co::IComponent* _componentType;

	/*
		Array of proxy interfaces created for the component facets. If '_facets'
		is NULL, it means this is a "Lua component type", not an instance.
	 */
	co::IService** _facets;

	// Length of the '_facets' array.
	int _numFacets;

	/*
		Lua registry reference to the component's table.
		If this instance is a Lua Component (type), _tableRef will be a
		reference to the component's prototype table. Otherwise, it will
		be a reference to an instance table.
	 */
	int _tableRef;

	// for the co::IDynamicServiceProvider methods to return values
	co::Any _res;
};

} // namespace lua

#endif
