/*
 * Coral - A lightweight C++ component framework
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

	/*!
		Looks for a Lua script with the given \a name in the Coral path.
		The search follows the same conventions used for locating Lua modules.
		Returns false if there is no script with the given name.
	 */
	static bool findScript( lua_State*L, const std::string& name, std::string& filename );

	//! Pushes a chunk loaded from a file. On error, raises a lua::Exception.
	static void loadFile( lua_State* L, const std::string& filename );

	/*!
		Pushes the result of <tt>require( moduleName )</tt> in Lua.
		\throw lua::Exception if \c require() signals an error.
	 */
	static void require( lua_State* L, const std::string& moduleName );

	/*!
		Calls a Lua function using the same conventions as lua_call().
		On error, raises a lua::Exception.
	 */
	static void call( lua_State* L, int numArgs, int numResults );

	//! Pushes any Coral value onto the Lua stack.
	//@{
	static void push( lua_State* L, const co::Any& var );
	static void push( lua_State* L, const co::Any& var, int depth );
	static void push( lua_State* L, const std::string& str );
	static void push( lua_State* L, co::Interface* itf );
	static void push( lua_State* L, co::Component* component );
	//@}

	/*!
		Gets any value from the Lua stack, using \a expectedType to help interpret it.
		If \a expectedType is NULL, this method will try to directly fit any Lua value into \a any.
		On the other hand, if the \a expectedType is a co::Any, the method will first call
		co::Any::createAny() on \a any and then try to fit a Lua value into the temporary co::Any.
		If the value at \a index has no convertion to Coral, raises a lua::Exception.
	 */
	static void getAny( lua_State* L, int index, co::Type* expectedType, co::Any& any );

	/*!
		Gets the value at \a index in the Lua stack and assigns it to \a outputVar.
		If the Lua value cannot be assigned to the var in \a outputVar, raises a lua::Exception.
		\warning \a outputVar must be a valid output variable.
	 */
	static void getValue( lua_State* L, int index, const co::Any& outputVar );

public:
    LuaState();
	virtual ~LuaState();

	bool findScript( const std::string& name, std::string& filename );

	co::int32 callFunction( const std::string& moduleName, const std::string& functionName,
								co::ArrayRange<const co::Any> args, co::ArrayRange<const co::Any> results );

private:
	template<typename BindingClass, typename InstanceType>
	static inline void pushInstance( lua_State* L, InstanceType* ptr );

	static void pushInstancesTable( lua_State* L );

	static void pushArray( lua_State* L, const co::Any& var );
	static void toArray( lua_State* L, int index, co::Type* elementType, co::Any& var );
	
	static co::int32 getEnumIdentifier( lua_State* L, int index, co::EnumType* enumType );

	static void checkType( lua_State* L, int index, int expectedType );
	static void raiseException( lua_State* L, int errorCode );

private:
	static lua_State* sm_L;
	static int sm_instancesTableRegIdx;
};

#endif
