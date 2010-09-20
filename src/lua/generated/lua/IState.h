/*******************************************************************************
** C++ mapping generated for type 'lua.IState'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _LUA_ISTATE_H_
#define _LUA_ISTATE_H_

#include <co/TypeTraits.h>
#include <co/Any.h>
#include <co/Interface.h>

// lua.IState Mapping:
namespace lua {

class IState : public co::Interface
{
public:
	virtual ~IState() {;}

	virtual void call( co::int32 numArgs, co::int32 numResults ) = 0;

	virtual void loadFile( const std::string& filename ) = 0;

	virtual void push( const co::Any& value ) = 0;

	virtual bool searchScript( const std::string& name, std::string& filename ) = 0;
};

} // namespace lua

namespace co {
template<> struct kindOf<lua::IState> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<lua::IState> { static const char* get() { return "lua.IState"; } };
template<> struct typeOf<lua::IState> : public typeOfBase<lua::IState, InterfaceType> {};
} // namespace co

#endif // _LUA_ISTATE_H_
