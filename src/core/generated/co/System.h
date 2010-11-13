/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_SYSTEM_H_
#define _CO_SYSTEM_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>
#include <vector>
#include <co/SystemState.h>

// Forward Declarations:
namespace co {
	class ModuleManager;
	class ServiceManager;
	class TypeManager;
} // namespace co
// End Of Forward Declarations

// co.System Mapping:
namespace co {

class System : public co::Interface
{
public:
	virtual ~System() {;}

	// Code From <c++ Block:
	
		//! Convenience method to setup the entire system without requiring any module.
		inline void setup() { setup( ArrayRange<std::string const>() ); }

		//! Convenience method to setup the entire system.
		inline void setup( ArrayRange<std::string const> requiredModules )
		{
			setupBase( requiredModules );
			setupPresentation();
		}
	
	// End Of c++> Block

	virtual co::ModuleManager* getModules() = 0;

	virtual co::ServiceManager* getServices() = 0;

	virtual co::SystemState getState() = 0;

	virtual co::TypeManager* getTypes() = 0;

	virtual void setupBase( co::ArrayRange<std::string const> requiredModules ) = 0;

	virtual void setupPresentation() = 0;

	virtual void tearDown() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::System> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::System> { static const char* get() { return "co.System"; } };
template<> struct typeOf<co::System> : public typeOfBase<co::System, InterfaceType> {};
} // namespace co

#endif // _CO_SYSTEM_H_
