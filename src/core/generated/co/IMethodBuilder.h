/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IMETHODBUILDER_H_
#define _CO_IMETHODBUILDER_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class IExceptionType;
	class IType;
	class ITypeBuilder;
} // namespace co
// End Of Forward Declarations

// co.IMethodBuilder Mapping:
namespace co {

class IMethodBuilder : public co::Interface
{
public:
	virtual ~IMethodBuilder() {;}

	virtual const std::string& getMethodName() = 0;

	virtual co::ITypeBuilder* getTypeBuilder() = 0;

	virtual void createMethod() = 0;

	virtual void defineException( co::IExceptionType* exceptionType ) = 0;

	virtual void defineParameter( const std::string& name, co::IType* type, bool input, bool output ) = 0;

	virtual void defineReturnType( co::IType* type ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IMethodBuilder> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IMethodBuilder> { static const char* get() { return "co.IMethodBuilder"; } };
template<> struct typeOf<co::IMethodBuilder> : public typeOfBase<co::IMethodBuilder, IInterfaceType> {};
} // namespace co

#endif // _CO_IMETHODBUILDER_H_
