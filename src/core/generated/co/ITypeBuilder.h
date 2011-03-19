/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ITYPEBUILDER_H_
#define _CO_ITYPEBUILDER_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>
#include <co/TypeKind.h>

// Forward Declarations:
namespace co {
	class IInterfaceType;
	class IMethodBuilder;
	class INamespace;
	class IType;
} // namespace co
// End Of Forward Declarations

// co.ITypeBuilder Mapping:
namespace co {

class ITypeBuilder : public co::Interface
{
public:
	virtual ~ITypeBuilder() {;}

	virtual co::TypeKind getKind() = 0;

	virtual co::INamespace* getNamespace() = 0;

	virtual const std::string& getTypeName() = 0;

	virtual co::IType* createType() = 0;

	virtual void defineAttribute( const std::string& name, co::IType* type, bool isReadOnly ) = 0;

	virtual void defineIdentifier( const std::string& name ) = 0;

	virtual void defineInterface( const std::string& name, co::IInterfaceType* interfaceType, bool isFacet ) = 0;

	virtual co::IMethodBuilder* defineMethod( const std::string& name ) = 0;

	virtual void defineNativeClass( const std::string& nativeHeaderFile, const std::string& nativeName ) = 0;

	virtual void defineSuperType( co::IType* superType ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ITypeBuilder> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ITypeBuilder> { static const char* get() { return "co.ITypeBuilder"; } };
template<> struct typeOf<co::ITypeBuilder> : public typeOfBase<co::ITypeBuilder, IInterfaceType> {};
} // namespace co

#endif // _CO_ITYPEBUILDER_H_
