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
	void setComponentType( co::IComponent* ct, int prototypeTableRef );
	void setComponentInstance( LuaComponent* prototype, int instanceTableRef );

	// co::IObject methods:
	co::IComponent* getComponentType();
	co::IService* getInterface( co::IPort* itfInfo );
	void setReceptacle( co::IPort* receptacle, co::IService* instance );

	// co::IDynamicServiceProvider methods:
	co::int32 registerProxyInterface( co::IService* proxy );
	const std::string& getProxyInterfaceName( co::int32 cookie );
	const co::Any& handleGetAttribute( co::int32 cookie, co::IField* ai );
	void handleSetAttribute( co::int32 cookie, co::IField* ai, const co::Any& value );
	const co::Any& handleMethodInvocation( co::int32 cookie, co::IMethod* mi, co::Range<co::Any const> args );

	// co::IReflector methods:
	co::int32 getSize();
	co::IType* getType();
	void createValue( void* address, size_t length );
    void copyValue( const void* fromAddress, void* toAddress );
    void destroyValue( void* address );
	co::IObject* newInstance();
	co::IService* newProxy( co::IDynamicServiceProvider* provider );
    void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value );
    void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value );
    void invokeMethod( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& returnValue );
	void raise( const std::string& message );

private:
	inline const std::string& getInterfaceName( co::int32 cookie );
	void pushInterfaceInstanceTable( lua_State* L, co::int32 cookie );
	void pushAccessorName( lua_State* L, const char* prefix, const std::string& attribName );
	void getMethod( lua_State* L, int t, co::int32 cookie = -1 );

	co::IService* getDynamicInterface( co::IPort* itfInfo );
	void bindToDynamicReceptacle( co::IPort* receptacle, co::IService* instance );

	void raiseNotSupportedException();

private:
	// co.IComponent that describes this Lua Component
	co::IComponent* _componentType;

	/*
		Array of proxy interfaces created for the component's facets. If '_facets'
		is NULL, it means this is a 'component type', not a 'component instance'.
	 */
	co::IService** _facets;

	// Length of the '_serverItfs' array.
	int _numFacets;

	/*
		Lua registry reference to the component's table.
		If this instance is a 'component type', '_tableRef' will be a reference to
		the component's prototype table. Otherwise, it will be a reference to
		the component's instance table.
	 */
	int _tableRef;

	// used by the co::IDynamicServiceProvider methods to return values
	co::Any _res;
};

} // namespace lua

#endif
