/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUABINDING_H_
#define _LUABINDING_H_

#include <lua.hpp>
#include <co/Any.h>

/*!
	Helper class for opening and closing the 'co' package, which
	contains the Coral global functions and the 'co.system' singleton.
 */
class coPackage
{
public:
	static void open( lua_State* L );
	static void close( lua_State* L );

private:
	static int newInstance( lua_State* L );
};

/*!
	Class with re-usable functions for binding co::CompoundTypes to Lua.
 */
class CompoundTypeBinding
{
public:
	/*!
		Given a userdata's index, attempts to retrieve a co::Any to any
		valid Coral object (i.e. an interface, native class, struct or component).
		If the userdata is not a valid Coral object, raises a lua::Exception.
	 */
	static void getInstance( lua_State* L, int index, co::Any& instance );

protected:
	//! Returns the literal type name of a co::CompoundType with the specified tag.
	static const char* getTypeName( co::TypeKind tag );

	//! Similar to luaL_checktype/luaL_typerror, but specialized for Coral userdata.
	static void checkType( lua_State* L, int narg, co::TypeKind expectedTag );

	/*!
		Pushes a metatable for a co::CompoundType's userdata.
		The userdata is cached in the registry under the specified 'key'.
		The co::CompoundType's concrete type is identified by the given 'tag'
	 */
	static void pushMetatable( lua_State* L, void* key, co::TypeKind tag );

	/*!
		Assumes the CompoundType's udata is at index 1 and the member name is at index 2.
		Pushes a function, if the member is a method; or a light userdata pointing to a
		co::MemberInfo, if the member is not a method. If the member cannot be found,
		pushes an error message and raises a lua::MsgOnStackException.
	 */
	static void pushMember( lua_State* L, co::CompoundType* ct );

	/*!
		Pops a light userdata (co::AttributeInfo*) from the stack.
		Pushes the corresponding attribute value of the given 'instance'.
	 */
	static void getAttribute( lua_State* L, const co::Any& instance );
	
	/*!
		Pops a value and a light userdata (co::AttributeInfo*) from the stack.
		Assigns the value to the corresponding attribute of the given 'instance'.
	 */
	static void setAttribute( lua_State* L, const co::Any& instance );

	/*!
		The lua_CFunction that dispatches method calls.
	 */
	static int callMethod( lua_State* L );
};

/*!
	Helper class for binding co::Interfaces to Lua.
 */
class InterfaceBinding : public CompoundTypeBinding
{
public:
	/*!
		Pushes a new instance of a co::Interface* userdata onto the stack.
	 */
	static void create( lua_State* L, co::Interface* itf );

	/*!
		Retrieves a co::Interface* from the userdata at the given index.
		For a userdata of the wrong type, generates a Lua error.
	 */
	static co::Interface* checkInstance( lua_State* L, int narg );

	// Metamethod Implementations:
	static int __index( lua_State* L );
	static int __newindex( lua_State* L );
	static int __gc( lua_State* L );
};

#endif
