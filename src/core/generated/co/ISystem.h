/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ISYSTEM_H_
#define _CO_ISYSTEM_H_

#include <co/TypeTraits.h>
#include <co/SystemState.h>
#include <co/Range.h>
#include <vector>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IModuleManager;
	class IServiceManager;
	class ITypeManager;
} // namespace co
// End Of Forward Declarations

// co.ISystem Mapping:
namespace co {

class ISystem : public co::IService
{
public:
	virtual ~ISystem() {;}

	//! Convenience method to setup the entire system without requiring any module.
	inline void setup() { setup( Range<std::string>() ); }
	
	//! Convenience method to setup the entire system.
	inline void setup( Range<std::string> requiredModules )
	{
		setupBase( requiredModules );
		setupPresentation();
	}

	virtual co::IModuleManager* getModules() = 0;

	virtual co::IServiceManager* getServices() = 0;

	virtual co::SystemState getState() = 0;

	virtual co::ITypeManager* getTypes() = 0;

	virtual void setupBase( co::Range<std::string> requiredModules ) = 0;

	virtual void setupPresentation() = 0;

	virtual void tearDown() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ISystem> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ISystem> { static const char* get() { return "co.ISystem"; } };
} // namespace co

#endif // _CO_ISYSTEM_H_
