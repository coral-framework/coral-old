/*******************************************************************************
** C++ mapping generated for type 'co.ParameterInfo'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_PARAMETERINFO_H_
#define _CO_PARAMETERINFO_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class Type;
} // namespace co
// End Of Forward Declarations

// co.ParameterInfo Mapping:
namespace co {

class ParameterInfo : public co::Interface
{
public:
	virtual ~ParameterInfo() {;}

	virtual bool getIsIn() = 0;

	virtual bool getIsOut() = 0;

	virtual const std::string& getName() = 0;

	virtual co::Type* getType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ParameterInfo> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ParameterInfo> { static const char* get() { return "co.ParameterInfo"; } };
template<> struct typeOf<co::ParameterInfo> : public typeOfBase<co::ParameterInfo, InterfaceType> {};
} // namespace co

#endif // _CO_PARAMETERINFO_H_
