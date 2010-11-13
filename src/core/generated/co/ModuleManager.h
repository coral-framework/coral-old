/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MODULEMANAGER_H_
#define _CO_MODULEMANAGER_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class Module;
	class ModulePartLoader;
} // namespace co
// End Of Forward Declarations

// co.ModuleManager Mapping:
namespace co {

class ModuleManager : public co::Interface
{
public:
	virtual ~ModuleManager() {;}

	virtual bool getBinaryCompatibilityChecking() = 0;

	virtual void setBinaryCompatibilityChecking( bool binaryCompatibilityChecking ) = 0;

	virtual co::ArrayRange<co::ModulePartLoader* const> getLoaders() = 0;

	virtual co::ArrayRange<co::Module* const> getModules() = 0;

	virtual co::Module* findModule( const std::string& moduleName ) = 0;

	virtual void installLoader( co::ModulePartLoader* loader ) = 0;

	virtual bool isLoadable( const std::string& moduleName ) = 0;

	virtual co::Module* load( const std::string& moduleName ) = 0;

	virtual void uninstallLoader( co::ModulePartLoader* loader ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ModuleManager> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ModuleManager> { static const char* get() { return "co.ModuleManager"; } };
template<> struct typeOf<co::ModuleManager> : public typeOfBase<co::ModuleManager, InterfaceType> {};
} // namespace co

#endif // _CO_MODULEMANAGER_H_
