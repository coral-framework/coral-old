/*******************************************************************************
** C++ mapping generated for type 'co.MethodInfo'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_METHODINFO_H_
#define _CO_METHODINFO_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/MemberInfo.h>

// Forward Declarations:
namespace co {
	class ExceptionType;
	class ParameterInfo;
	class Type;
} // namespace co
// End Of Forward Declarations

// co.MethodInfo Mapping:
namespace co {

class MethodInfo : public co::MemberInfo
{
public:
	virtual ~MethodInfo() {;}

	virtual co::ArrayRange<co::ExceptionType* const> getExceptions() = 0;

	virtual co::ArrayRange<co::ParameterInfo* const> getParameters() = 0;

	virtual co::Type* getReturnType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::MethodInfo> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::MethodInfo> { static const char* get() { return "co.MethodInfo"; } };
template<> struct typeOf<co::MethodInfo> : public typeOfBase<co::MethodInfo, InterfaceType> {};
} // namespace co

#endif // _CO_METHODINFO_H_
