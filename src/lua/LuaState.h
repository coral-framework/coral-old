/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUASTATE_H_
#define _LUASTATE_H_

#include <lua.hpp>
#include <co/Any.h>

/*!
	Helper struct for reading values from Lua.
 */
struct LuaValue
{
	co::Any any;

	/*!
		Storage area for strings retrieved from Lua (since Coral
		expects strings to be passed as 'const std::string&'s).

		Here we assume the STL implementation doesn't allocate
		memory in the default std::string constructor.
	 */
	std::string str;
};

/*!
	Singleton that manages a Lua universe.
 */
class LuaState
{
public:
	static inline LuaState& instance() { return sm_instance; }

public:
    LuaState();
	~LuaState();

	void setup();
	void tearDown();

	//! Returns the underlying lua_State*.
	inline lua_State* get() const { return _L; }

	//! Loads a chunk from a file. On error, raises a lua::Exception.
	void loadFile( const std::string& filename );

	//! Calls the Lua function on the stack top. On error, raises a lua::Exception.
	void call( int numArgs, int numResults );

	//! Pushes any Coral value onto the stack.
	//@{
	void push( const co::Any& any );
	void push( const std::string& str );
	void push( co::Interface* itf );
	//@}

	/*!
		Reads a Lua value from the stack. Or, if the value at the given
		stack index is not a valid Coral value, raises a lua::Exception.
	 */
	void toCoral( int index, LuaValue& value );

private:
	void pushInstancesTable();
	void raiseException( int errorCode );

private:
	static LuaState sm_instance;

private:
	lua_State* _L;
	int _instancesTableRegIdx;
};

#endif
