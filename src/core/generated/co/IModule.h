/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IMODULE_H_
#define _CO_IMODULE_H_

#include <co/TypeTraits.h>
#include <co/Range.h>
#include <co/ModuleState.h>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IModulePart;
	class INamespace;
} // namespace co
// End Of Forward Declarations

// co.IModule Mapping:
namespace co {

class IModule : public co::IService
{
public:
	virtual ~IModule() {;}

	virtual co::INamespace* getNamespace() = 0;

	virtual co::Range<co::IModulePart* const> getParts() = 0;

	virtual co::int32 getRank() = 0;

	virtual void setRank( co::int32 rank ) = 0;

	virtual co::ModuleState getState() = 0;

	virtual void abort() = 0;

	virtual void disintegrate() = 0;

	virtual void dispose() = 0;

	virtual void initialize() = 0;

	virtual void integrate() = 0;

	virtual void integratePresentation() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IModule> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IModule> { static const char* get() { return "co.IModule"; } };
} // namespace co

#endif // _CO_IMODULE_H_
