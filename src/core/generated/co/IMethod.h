/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IMETHOD_H_
#define _CO_IMETHOD_H_

#include <co/TypeTraits.h>
#include <co/IMember.h>
#include <co/Range.h>

// Forward Declarations:
namespace co {
	class IException;
	class IParameter;
	class IType;
} // namespace co
// End Of Forward Declarations

// co.IMethod Mapping:
namespace co {

class IMethod : public co::IMember
{
public:
	virtual ~IMethod() {;}

	virtual co::Range<co::IException* const> getExceptions() = 0;

	virtual co::Range<co::IParameter* const> getParameters() = 0;

	virtual co::IType* getReturnType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IMethod> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IMethod> { static const char* get() { return "co.IMethod"; } };
template<> struct typeOf<co::IMethod> : public typeOfBase<co::IMethod, IInterface> {};
} // namespace co

#endif // _CO_IMETHOD_H_
