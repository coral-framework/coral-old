/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUASTATE_H_
#define _LUASTATE_H_

#include "Universe_Base.h"
#include <lua.hpp>

/*!
	A static component class that manages a Lua universe and
	provides methods to exchange values between Lua and Coral.
 
	All instances of this component share the same state.
 
	\warning
	All functions taking a lua_State in this module must be called with either
	the lua_State returned by LuaState::getL(), or a Lua thread	created by it.
	Currently we do NOT support working with multiple independent lua_States.
 */
class LuaState : public lua::Universe_Base
{
public:
	//! Returns our Lua universe (a main thread).
	static inline lua_State* getL() { return sm_L; }

	//! Creates and sets up our Lua universe.
	static void setup();

	//! Closes our Lua universe.
	static void tearDown();

	//! Debug method: prints the Lua stack to stdout.
	static void dumpStack( lua_State* L );

	//! Loads and runs the given string. On error, raises a lua::Exception.
	static void doString( lua_State* L, const char* code );

	/*!
		Searches for a Lua file with the given 'name' in the CORAL_PATH.
		The search follows the same conventions used for locating Lua packages.
		Returns false if no script could be found.
	 */
	static bool searchScriptFile( const std::string& name, std::string& filename );

	//! Pushes a chunk loaded from a file. On error, raises a lua::Exception.
	static void loadFile( lua_State* L, const std::string& filename );

	//! Pushes a chunk loaded from a string. On error, raises a lua::Exception.
	static void loadString( lua_State* L, const char* code );

	/*!
		Calls a Lua function using the same conventions as lua_call().
		On error, raises a lua::Exception.
	 */
	static void call( lua_State* L, co::int32 numArgs, co::int32 numResults );

	//! Pushes any Coral value onto the Lua stack.
	//@{
	static void push( lua_State* L, const co::Any& var );
	static void push( lua_State* L, const co::Any& var, int depth );
	static void push( lua_State* L, const std::string& str );
	static void push( lua_State* L, co::Interface* itf );
	static void push( lua_State* L, co::Component* component );
	//@}

	/*!
		Reads a value from the stack, using \c expectedType to help interpret the value.
		The result is returned in \c var. If the value at the specified index is not a
		valid Coral value, raises a lua::Exception.
	 */
	static void toCoral( lua_State* L, int index, co::Type* expectedType, co::Any& var );

public:
    LuaState();
	virtual ~LuaState();

	// These methods delegate calls to their static counterparts, with L = LuaState::getL().
	bool searchScript( const std::string& name, std::string& filename );
	void loadFile( const std::string& filename );
	void call( co::int32 numArgs, co::int32 numResults );
	void push( const co::Any& var );

private:
	template<typename BindingClass, typename InstanceType>
	static inline void pushInstance( lua_State* L, InstanceType* ptr );

	static void pushInstancesTable( lua_State* L );

	static void pushArray( lua_State* L, const co::Any& var );
	static void toArray( lua_State* L, int index, co::Type* expectedType, co::Any& var );

	static void raiseException( lua_State* L, int errorCode );

private:
	static lua_State* sm_L;
	static int sm_instancesTableRegIdx;
};

#endif
