/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IMETHODBUILDER_H_
#define _CO_IMETHODBUILDER_H_

#include <co/TypeTraits.h>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IException;
	class IType;
	class ITypeBuilder;
} // namespace co
// End Of Forward Declarations

// co.IMethodBuilder Mapping:
namespace co {

class IMethodBuilder : public co::IService
{
public:
	virtual ~IMethodBuilder() {;}

	virtual std::string getMethodName() = 0;

	virtual co::ITypeBuilder* getTypeBuilder() = 0;

	virtual void createMethod() = 0;

	virtual void defineException( co::IException* exceptionType ) = 0;

	virtual void defineParameter( const std::string& name, co::IType* type, bool input, bool output ) = 0;

	virtual void defineReturnType( co::IType* type ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IMethodBuilder> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IMethodBuilder> { static const char* get() { return "co.IMethodBuilder"; } };
} // namespace co

#endif // _CO_IMETHODBUILDER_H_
