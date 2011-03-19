/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IMODULEMANAGER_H_
#define _CO_IMODULEMANAGER_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class IModule;
	class IModulePartLoader;
} // namespace co
// End Of Forward Declarations

// co.IModuleManager Mapping:
namespace co {

class IModuleManager : public co::Interface
{
public:
	virtual ~IModuleManager() {;}

	virtual bool getBinaryCompatibilityChecking() = 0;

	virtual void setBinaryCompatibilityChecking( bool binaryCompatibilityChecking ) = 0;

	virtual co::ArrayRange<co::IModulePartLoader* const> getLoaders() = 0;

	virtual co::ArrayRange<co::IModule* const> getModules() = 0;

	virtual co::IModule* findModule( const std::string& moduleName ) = 0;

	virtual void installLoader( co::IModulePartLoader* loader ) = 0;

	virtual bool isLoadable( const std::string& moduleName ) = 0;

	virtual co::IModule* load( const std::string& moduleName ) = 0;

	virtual void uninstallLoader( co::IModulePartLoader* loader ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IModuleManager> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IModuleManager> { static const char* get() { return "co.IModuleManager"; } };
template<> struct typeOf<co::IModuleManager> : public typeOfBase<co::IModuleManager, IInterfaceType> {};
} // namespace co

#endif // _CO_IMODULEMANAGER_H_
