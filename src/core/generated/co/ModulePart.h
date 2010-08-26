/*******************************************************************************
** C++ mapping generated for type 'co.ModulePart'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_MODULEPART_H_
#define _CO_MODULEPART_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class Module;
} // namespace co
// End Of Forward Declarations

// co.ModulePart Mapping:
namespace co {

class ModulePart : public co::Interface
{
public:
	virtual ~ModulePart() {;}

	virtual void disintegrate( co::Module* module ) = 0;

	virtual void dispose( co::Module* module ) = 0;

	virtual void initialize( co::Module* module ) = 0;

	virtual void integrate( co::Module* module ) = 0;

	virtual void integratePresentation( co::Module* module ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ModulePart> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ModulePart> { static const char* get() { return "co.ModulePart"; } };
template<> struct typeOf<co::ModulePart> : public typeOfBase<co::ModulePart, InterfaceType> {};
} // namespace co

#endif // _CO_MODULEPART_H_
