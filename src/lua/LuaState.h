/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUASTATE_H_
#define _LUASTATE_H_

#include <co/AnyValue.h>
#include <lua.hpp>

/*!
	Singleton that manages a Lua universe and provides
	methods to exchange values between Lua and Coral.
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

	//! Debug method: prints the Lua stack to stdout.
	void dumpStack();

	//! Loads a chunk from a file. On error, raises a lua::Exception.
	void loadFile( const std::string& filename );

	//! Calls the Lua function on the stack top. On error, raises a lua::Exception.
	void call( int numArgs, int numResults );

	//! Pushes any Coral value onto the stack.
	//@{
	void push( const co::Any& var, int depth = 0 );
	void push( const std::string& str );
	void push( co::Interface* itf );
	void push( co::Component* component );
	//@}

	/*!
		Reads a value from the stack, using \c expectedType to help interpret the value.
		The result is returned in \c variable. Non-primitive values are allocated in \c value.
		If the value at the specified index is not a valid Coral value, raises a lua::Exception.
	 */
	void toCoral( int index, co::Type* expectedType, co::Any& var, co::AnyValue& value );

private:
	template<typename BindingClass, typename InstanceType>
	inline void pushInstance( InstanceType* ptr );

	void pushInstancesTable();

	void pushArray( const co::Any& var );
	void toArray( int index, co::Type* expectedType, co::Any& var, co::AnyValue& value );

	void raiseException( int errorCode );

private:
	static LuaState sm_instance;

private:
	lua_State* _L;
	int _instancesTableRegIdx;
};

#endif
