/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ITYPEBUILDER_H_
#define _CO_ITYPEBUILDER_H_

#include <co/TypeTraits.h>
#include <co/IService.h>
#include <co/TypeKind.h>

// Forward Declarations:
namespace co {
	class IInterface;
	class IMethodBuilder;
	class INamespace;
	class IType;
} // namespace co
// End Of Forward Declarations

// co.ITypeBuilder Mapping:
namespace co {

class ITypeBuilder : public co::IService
{
public:
	virtual ~ITypeBuilder() {;}

	virtual co::TypeKind getKind() = 0;

	virtual co::INamespace* getNamespace() = 0;

	virtual const std::string& getTypeName() = 0;

	virtual co::IType* createType() = 0;

	virtual void defineBaseType( co::IType* baseType ) = 0;

	virtual void defineField( const std::string& name, co::IType* type, bool isReadOnly ) = 0;

	virtual void defineIdentifier( const std::string& name ) = 0;

	virtual co::IMethodBuilder* defineMethod( const std::string& name ) = 0;

	virtual void defineNativeClass( const std::string& nativeHeader, const std::string& nativeName ) = 0;

	virtual void definePort( const std::string& name, co::IInterface* type, bool isFacet ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ITypeBuilder> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ITypeBuilder> { static const char* get() { return "co.ITypeBuilder"; } };
} // namespace co

#endif // _CO_ITYPEBUILDER_H_
