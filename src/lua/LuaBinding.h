/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUABINDING_H_
#define _LUABINDING_H_

#include <lua.hpp>
#include <co/Any.h>
#include <co/Component.h>

/*!
	Static class for opening and closing the 'co' package, which
	contains the Coral global functions and the 'co.system' singleton.
 */
class coPackage
{
public:
	static void open( lua_State* L );
	static void close( lua_State* L );

private:
	static int addPath( lua_State* L );
	static int getPaths( lua_State* L );
	static int getType( lua_State* L );
	static int genericNew( lua_State* L );
	static int packageLoader( lua_State* L );
};

/*!
	Auxiliary static class containing re-usable functions for
	binding co::CompoundTypes to Lua.
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

	/*!
		Just like getInstance(), but does not raise exceptions (if the userdata
		is invalid, 'instance' is left unmodified).
	 */
	static void tryGetInstance( lua_State* L, int index, co::Any& instance );

protected:
	/*!
		Pushes a metatable for a userdata of the specified co::CompoundType.
		The metatable is cached in the registry, indexed by the co::CompoundType pointer.
	 */
	static void pushMetatable( lua_State* L, co::CompoundType* ct );

	/*!
		Given a userdata's index, attempts to get its co::CompoundType* (from its
		metatable). Returns NULL if the userdata is not a valid Coral object.
	 */
	static co::CompoundType* getType( lua_State* L, int index );

	/*!
		Assumes the CompoundType's udata is at index 1 and the member name is at index 2.
		Pushes a function and returns true, if the member is a method; or a light
		userdata pointing to a co::MemberInfo, if the member is not a method.
		If the member cannot be found and \c mustExist is false (default), pushes nil and
		returns false; otherwise, if \c mustExist is true, an exception is thrown.
	 */
	static bool pushMember( lua_State* L, co::CompoundType* ct, bool mustExist = false );

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
	Static class for binding co::Components to Lua.
 */
class ComponentBinding : public CompoundTypeBinding
{
public:
	//! Pushes a new instance of a co::Component* userdata onto the stack.
	static void create( lua_State* L, co::Component* component );

	// --- Metamethods ---
	static int index( lua_State* L );
	static int newIndex( lua_State* L );
	static int gc( lua_State* L );
	static int toString( lua_State* L );
};

/*!
	Static class for binding co::Interfaces to Lua.
 */
class InterfaceBinding : public CompoundTypeBinding
{
public:
	//! Pushes a new instance of a co::Interface* userdata onto the stack.
	static void create( lua_State* L, co::Interface* itf );

	// --- Metamethods ---
	static int index( lua_State* L );
	static int newIndex( lua_State* L );
	static int gc( lua_State* L );
	static int toString( lua_State* L );
};

/*!
	Re-usable class for binding complex values to Lua.
 */
class ComplexValueBinding : public CompoundTypeBinding
{
public:
	/*!
		Pushes a userdata of the specified complex value type onto the stack.
		If an instancePtr is provided, the pushed complex value will be a copy of it.
		Otherwise, if instancePtr is NULL, a default-constructed complex value will be pushed.
	 */
	static void push( lua_State* L, co::Type* type, void* instancePtr );

	// --- Metamethods ---
	static int gc( lua_State* L );
	static int toString( lua_State* L );
};

/*!
	Static class for binding native classes to Lua.
 */
class NativeClassBinding : public ComplexValueBinding
{
public:
	// --- Metamethods ---
	static int index( lua_State* L );
	static int newIndex( lua_State* L );
};

/*!
	Static class for binding structs to Lua.
 */
class StructBinding : public ComplexValueBinding
{
public:
	// --- Metamethods ---
	static int index( lua_State* L );
	static int newIndex( lua_State* L );
};

#endif
