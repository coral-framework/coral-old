/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUASTATE_H_
#define _LUASTATE_H_

#include <lua.hpp>
#include <co/Any.h>

class Variant;

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

/*!
	Auxiliary struct for reading values from Lua.
 */
struct Variant
{
	union
	{
		co::__any::State::Data data;
		co::uint8 __anyArea[sizeof(co::Any)];
		co::uint8 __stringArea[sizeof(std::string)];
	};

	co::TypeKind kind;

	inline Variant() : kind( co::TK_NONE )
	{
		data.ptr = NULL;
	}

	inline ~Variant()
	{
		clear();
	}

	inline void clear()
	{
		switch( kind )
		{
		case co::TK_ANY:
			reinterpret_cast<co::Any*>( __anyArea )->~Any();
			break;
		case co::TK_STRING:
			reinterpret_cast<std::string*>( __stringArea )->~basic_string();
			break;
		default:
			break;
		}
	}
	
	inline void createAny()
	{
		clear();
		new( __anyArea ) co::Any();
		kind = co::TK_ANY;
	}

	inline void createString()
	{
		clear();
		new( __stringArea ) std::string();
		kind = co::TK_STRING;
	}

	inline co::Any& getAny()
	{
		assert( kind == co::TK_ANY );
		return *reinterpret_cast<co::Any*>( __anyArea );
	}

	inline std::string& getString()
	{
		assert( kind == co::TK_STRING );
		return *reinterpret_cast<std::string*>( __stringArea );
	}

	/*!
		Prepares a {co::Any,Variant} pair for use as an 'out' argument of the given type.
		\param type the 'out' parameter type.
		\param arg the co::Any that will be passed to the method; it may contain
					a value (that must be preserved) if the parameter is 'inout'.
		This method is allowed to raise exceptions.
	 */
	void makeOut( co::Type* type, co::Any& arg );

	/*!
		Converts a {co::Any,Variant} pair that's currently an 'out' argument into an 'in' argument.
		\param arg the co::Any that will be passed to the method.
		This method is NOT allowed to raise exceptions.
	 */
	void makeIn( co::Any& arg );
};

#endif
