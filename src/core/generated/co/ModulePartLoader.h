/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MODULEPARTLOADER_H_
#define _CO_MODULEPARTLOADER_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class ModulePart;
} // namespace co
// End Of Forward Declarations

// co.ModulePartLoader Mapping:
namespace co {

class ModulePartLoader : public co::Interface
{
public:
	virtual ~ModulePartLoader() {;}

	virtual bool canLoadModulePart( const std::string& moduleName ) = 0;

	virtual co::ModulePart* loadModulePart( const std::string& moduleName ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ModulePartLoader> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ModulePartLoader> { static const char* get() { return "co.ModulePartLoader"; } };
template<> struct typeOf<co::ModulePartLoader> : public typeOfBase<co::ModulePartLoader, InterfaceType> {};
} // namespace co

#endif // _CO_MODULEPARTLOADER_H_
