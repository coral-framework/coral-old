/*******************************************************************************
** C++ mapping generated for type 'co.TypeBuilder'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_TYPEBUILDER_H_
#define _CO_TYPEBUILDER_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>
#include <co/TypeKind.h>

// Forward Declarations:
namespace co {
	class InterfaceType;
	class MethodBuilder;
	class Namespace;
	class Type;
} // namespace co
// End Of Forward Declarations

// co.TypeBuilder Mapping:
namespace co {

class TypeBuilder : public co::Interface
{
public:
	virtual ~TypeBuilder() {;}

	virtual co::TypeKind getKind() = 0;

	virtual co::Namespace* getNamespace() = 0;

	virtual const std::string& getTypeName() = 0;

	virtual co::Type* createType() = 0;

	virtual void defineAttribute( const std::string& name, co::Type* type, bool isReadOnly ) = 0;

	virtual void defineIdentifier( const std::string& name ) = 0;

	virtual void defineInterface( const std::string& name, co::InterfaceType* interfaceType, bool isProvided ) = 0;

	virtual co::MethodBuilder* defineMethod( const std::string& name ) = 0;

	virtual void defineNativeClass( const std::string& nativeName, const std::string& headerName ) = 0;

	virtual void defineSuperType( co::Type* superType ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::TypeBuilder> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::TypeBuilder> { static const char* get() { return "co.TypeBuilder"; } };
template<> struct typeOf<co::TypeBuilder> : public typeOfBase<co::TypeBuilder, InterfaceType> {};
} // namespace co

#endif // _CO_TYPEBUILDER_H_
