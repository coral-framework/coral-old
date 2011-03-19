/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ISYSTEM_H_
#define _CO_ISYSTEM_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>
#include <vector>
#include <co/SystemState.h>

// Forward Declarations:
namespace co {
	class IModuleManager;
	class IServiceManager;
	class ITypeManager;
} // namespace co
// End Of Forward Declarations

// co.ISystem Mapping:
namespace co {

class ISystem : public co::Interface
{
public:
	virtual ~ISystem() {;}

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

	virtual co::IModuleManager* getModules() = 0;

	virtual co::IServiceManager* getServices() = 0;

	virtual co::SystemState getState() = 0;

	virtual co::ITypeManager* getTypes() = 0;

	virtual void setupBase( co::ArrayRange<std::string const> requiredModules ) = 0;

	virtual void setupPresentation() = 0;

	virtual void tearDown() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ISystem> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ISystem> { static const char* get() { return "co.ISystem"; } };
template<> struct typeOf<co::ISystem> : public typeOfBase<co::ISystem, IInterfaceType> {};
} // namespace co

#endif // _CO_ISYSTEM_H_
