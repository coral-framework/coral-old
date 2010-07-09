/*******************************************************************************
** C++ mapping generated for type 'co.Module'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_MODULE_H_
#define _CO_MODULE_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>
#include <co/ModuleState.h>

// Forward Declarations:
namespace co {
	class ModulePart;
	class Namespace;
} // namespace co
// End Of Forward Declarations

// co.Module Mapping:
namespace co {

class Module : public co::Interface
{
public:
	virtual ~Module() {;}

	virtual co::Namespace* getNamespace() = 0;

	virtual co::ArrayRange<co::ModulePart* const> getParts() = 0;

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
template<> struct kindOf<co::Module> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::Module> { static const char* get() { return "co.Module"; } };
template<> struct typeOf<co::Module> : public typeOfBase<co::Module, InterfaceType> {};
} // namespace co

#endif // _CO_MODULE_H_
