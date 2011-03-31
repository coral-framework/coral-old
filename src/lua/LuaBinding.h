/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUABINDING_H_
#define _LUABINDING_H_

#include <lua.hpp>
#include <co/Any.h>
#include <co/IObject.h>

namespace lua {

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
	static int findScript( lua_State* L );
	static int findModuleFile( lua_State* L );
	static int getType( lua_State* L );
	static int genericNew( lua_State* L );
	static int newComponentType( lua_State* L );
	static int newComponentInstance( lua_State* L );

private:
	typedef std::vector<co::IComponent*> ComponentTypeList;
	static ComponentTypeList sm_luaComponentTypes;
};

/*!
	Auxiliary static class containing re-usable functions for
	binding co::CompositeTypes to Lua.
 */
class CompositeTypeBinding
{
public:
	/*!
		Attempts to get the co::ICompositeType* for the Coral object at \a index.
		Returns NULL if the value at \a index is not a valid Coral object.
	 */
	static co::ICompositeType* getType( lua_State* L, int index );

	/*!
		Given a userdata's index, attempts to set a co::Any with a reference to
		any valid Coral object (interface, native class, struct or component).
		If the userdata is not a valid Coral object, raises a lua::Exception.
	 */
	static void getInstance( lua_State* L, int index, co::Any& instance );

	/*!
		 Just like getInstance(), but does not raise exceptions. If the userdata
		 is invalid, 'instance' is left unmodified and the method returns false.
	 */
	static bool tryGetInstance( lua_State* L, int index, co::Any& instance );

	/*!
		Removes all references to ICompositeType metatables from the Lua registry.
	 */
	static void releaseBindings( lua_State* L );

protected:
	/*!
		Pushes a metatable for a userdata of the specified co::ICompositeType.
		The metatable is cached in the registry, indexed by the co::ICompositeType pointer.
	 */
	static void pushMetatable( lua_State* L, co::ICompositeType* ct, co::IReflector* reflector = 0 );

	/*!
		Assumes the ICompositeType's udata is at index 1 and the member name is at index 2.
		Pushes a function and returns true, if the member is a method; or a light
		userdata pointing to a co::IMember, if the member is not a method.
		If the member cannot be found and \a mustExist is false (default), pushes nil and
		returns false; otherwise, if \a mustExist is true, an exception is thrown.
	 */
	static bool pushMember( lua_State* L, co::ICompositeType* ct, bool mustExist = false );

	/*!
		Pops a light userdata (co::IField*) from the stack.
		Pushes the corresponding field value of the given 'instance'.
	 */
	static void getField( lua_State* L, const co::Any& instance );

	/*!
		Pops a value and a light userdata (co::IField*) from the stack.
		Assigns the value to the corresponding field of the given 'instance'.
	 */
	static void setField( lua_State* L, const co::Any& instance );

	/*!
		The lua_CFunction that dispatches method calls.
	 */
	static int callMethod( lua_State* L );

private:
	typedef std::vector<co::ICompositeType*> CompositeTypeList;
	static CompositeTypeList sm_boundTypes;
};

/*!
	Static class for binding co::Components to Lua.
 */
class ComponentBinding : public CompositeTypeBinding
{
public:
	//! Pushes a new instance of a co::IObject* userdata onto the stack.
	static void create( lua_State* L, co::IObject* object );

	// --- Metamethods ---
	static int index( lua_State* L );
	static int newIndex( lua_State* L );
	static int gc( lua_State* L );
	static int toString( lua_State* L );
};

/*!
	Static class for binding co::Interfaces to Lua.
 */
class InterfaceBinding : public CompositeTypeBinding
{
public:
	//! Pushes a new instance of a co::IService* userdata onto the stack.
	static void create( lua_State* L, co::IService* itf );

	//! Gets the interface pointer of a verified Coral interface userdata.
	inline static co::IService* getInstance( lua_State* L, int index )
	{
		return *reinterpret_cast<co::IService**>( lua_touserdata( L, index ) );
	}

	// --- Metamethods ---
	static int index( lua_State* L );
	static int newIndex( lua_State* L );
	static int gc( lua_State* L );
	static int toString( lua_State* L );
};

/*!
	Re-usable class for binding complex values to Lua.
 */
class ComplexValueBinding : public CompositeTypeBinding
{
public:
	/*!
		Pushes a userdata of the specified complex value type onto the stack.
		If an instancePtr is provided, the pushed complex value will be a copy of it.
		Otherwise, if instancePtr is NULL, a default-constructed complex value will be pushed.
	 */
	static void push( lua_State* L, co::IType* type, void* instancePtr );

	//! Gets the instance pointer of a verified Coral CV userdata.
	inline static void* getInstance( lua_State* L, int index )
	{
		return lua_touserdata( L, index );
	}

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

} // namespace lua

#endif
