/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUASTATE_H_
#define _LUASTATE_H_

#include "Variant.h"
#include <lua.hpp>
#include <co/Any.h>

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
	void push( const co::Any& any, int depth = 0 );
	void push( const std::string& str );
	void push( co::Interface* itf );
	void push( co::Component* component );
	//@}

	/*!
		Reads a value from the stack, using \c expectedType to help interpret the value.
		If the value at the specified stack index is not a valid Coral value,
		raises a lua::Exception. The Variant \c data is a support parameter,
		used for storage of values that do not fit in a co::Any (i.e. strings).
	 */
	void toCoral( int index, co::Type* expectedType, co::Any& value, Variant& data );

private:
	void pushArray( const co::__any::State& s );

	template<typename BindingClass, typename InstanceType>
	inline void pushInstance( InstanceType* ptr );

	void pushInstancesTable();
	void raiseException( int errorCode );

private:
	static LuaState sm_instance;

private:
	lua_State* _L;
	int _instancesTableRegIdx;
};

#endif
