/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUACOMPONENT_H_
#define _LUACOMPONENT_H_

#include "Component_Base.h"
#include <co/RefPtr.h>

/*!
	A dynamic component, with interfaces defined in run-time and methods implemented in Lua.
 */
class LuaComponent : public lua::Component_Base
{
public:
    LuaComponent();
	virtual ~LuaComponent();

	// internal methods:
	void setComponentType( co::ComponentType* ct, int prototypeTableRef );
	void setComponentInstance( int instanceTableRef );

	// co::Component methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* itfInfo );
	void bindInterface( co::InterfaceInfo* clientItfInfo, co::Interface* instance );

	// co::DynamicProxyHandler methods:
	co::int32 registerProxyInterface( co::Interface* proxy );
	const std::string& getProxyInterfaceName( co::int32 cookie );
	void handleGetAttribute( co::int32 cookie, co::AttributeInfo* ai, co::Any& value );
	void handleSetAttribute( co::int32 cookie, co::AttributeInfo* ai, const co::Any& value );
	void handleMethodInvocation( co::int32 cookie, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& returnValue );

	// co::Reflector methods:
	co::int32 getSize();
	co::Type* getType();
	void createValue( void* address, size_t length );
    void copyValue( const void* fromAddress, void* toAddress );
    void destroyValue( void* address );
	co::Component* newInstance();
	co::Interface* newProxy( co::DynamicProxyHandler* handler );
    void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value );
    void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value );
    void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& returnValue );

private:
	co::Interface* getCustomInterface( co::InterfaceInfo* itfInfo );
	void bindCustomInterface( co::InterfaceInfo* clientItfInfo, co::Interface* instance );

	void raiseUnsupportedOperationException();

private:
	// co.ComponentType that describes this Lua Component
	co::ComponentType* _customType;

	/*
		Array of server proxy interfaces created for the component.
		If '_serverItfs' is NULL, it means this lua::Component is a 'component type',
		not a 'component instance'.
	 */
	co::Interface** _serverItfs;

	// Length of the '_serverItfs' array.
	int _numServerItfs;

	/*
		Lua registry reference to the component's table.
		If this instance is a 'component type', '_tableRef' will be a reference to
		the component's prototype table. Otherwise, it will be a reference to
		the component's instance table.
	 */
	int _tableRef;
};

#endif
