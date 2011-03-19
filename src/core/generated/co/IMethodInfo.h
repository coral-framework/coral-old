/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IMETHODINFO_H_
#define _CO_IMETHODINFO_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/IMemberInfo.h>

// Forward Declarations:
namespace co {
	class IExceptionType;
	class IParameterInfo;
	class IType;
} // namespace co
// End Of Forward Declarations

// co.IMethodInfo Mapping:
namespace co {

class IMethodInfo : public co::IMemberInfo
{
public:
	virtual ~IMethodInfo() {;}

	virtual co::ArrayRange<co::IExceptionType* const> getExceptions() = 0;

	virtual co::ArrayRange<co::IParameterInfo* const> getParameters() = 0;

	virtual co::IType* getReturnType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IMethodInfo> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IMethodInfo> { static const char* get() { return "co.IMethodInfo"; } };
template<> struct typeOf<co::IMethodInfo> : public typeOfBase<co::IMethodInfo, IInterfaceType> {};
} // namespace co

#endif // _CO_IMETHODINFO_H_
