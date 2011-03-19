/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IPARAMETERINFO_H_
#define _CO_IPARAMETERINFO_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class IType;
} // namespace co
// End Of Forward Declarations

// co.IParameterInfo Mapping:
namespace co {

class IParameterInfo : public co::Interface
{
public:
	virtual ~IParameterInfo() {;}

	virtual bool getIsIn() = 0;

	virtual bool getIsOut() = 0;

	virtual const std::string& getName() = 0;

	virtual co::IType* getType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IParameterInfo> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IParameterInfo> { static const char* get() { return "co.IParameterInfo"; } };
template<> struct typeOf<co::IParameterInfo> : public typeOfBase<co::IParameterInfo, IInterfaceType> {};
} // namespace co

#endif // _CO_IPARAMETERINFO_H_
