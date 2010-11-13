/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_METHODBUILDER_H_
#define _CO_METHODBUILDER_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class ExceptionType;
	class Type;
	class TypeBuilder;
} // namespace co
// End Of Forward Declarations

// co.MethodBuilder Mapping:
namespace co {

class MethodBuilder : public co::Interface
{
public:
	virtual ~MethodBuilder() {;}

	virtual const std::string& getMethodName() = 0;

	virtual co::TypeBuilder* getTypeBuilder() = 0;

	virtual void createMethod() = 0;

	virtual void defineException( co::ExceptionType* exceptionType ) = 0;

	virtual void defineParameter( const std::string& name, co::Type* type, bool input, bool output ) = 0;

	virtual void defineReturnType( co::Type* type ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::MethodBuilder> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::MethodBuilder> { static const char* get() { return "co.MethodBuilder"; } };
template<> struct typeOf<co::MethodBuilder> : public typeOfBase<co::MethodBuilder, InterfaceType> {};
} // namespace co

#endif // _CO_METHODBUILDER_H_
