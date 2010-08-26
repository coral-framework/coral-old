/*******************************************************************************
** C++ mapping generated for type 'co.ModuleState'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_MODULESTATE_H_
#define _CO_MODULESTATE_H_

#include <co/TypeTraits.h>

// co.ModuleState Mapping:
namespace co {

enum ModuleState
{
	ModuleState_None,
	ModuleState_Initialized,
	ModuleState_Integrated,
	ModuleState_PresentationIntegrated,
	ModuleState_Disintegrated,
	ModuleState_Disposed,
	ModuleState_Aborted,
	__ModuleState__FORCE_SIZEOF_UINT32 = 0xFFFFFFFF
};

} // namespace co

namespace co {
template<> struct kindOf<co::ModuleState> : public kindOfBase<TK_ENUM> {};
template<> struct nameOf<co::ModuleState> { static const char* get() { return "co.ModuleState"; } };
template<> struct typeOf<co::ModuleState> : public typeOfBase<co::ModuleState, EnumType> {};
} // namespace co

#endif // _CO_MODULESTATE_H_
