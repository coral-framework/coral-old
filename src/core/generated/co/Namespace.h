/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_NAMESPACE_H_
#define _CO_NAMESPACE_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>
#include <co/TypeKind.h>
#include <co/Namespace.h>

// Forward Declarations:
namespace co {
	class Module;
	class Type;
	class TypeBuilder;
	class TypeCreationTransaction;
} // namespace co
// End Of Forward Declarations

// co.Namespace Mapping:
namespace co {

class Namespace : public co::Interface
{
public:
	virtual ~Namespace() {;}

	virtual co::ArrayRange<co::Namespace* const> getChildNamespaces() = 0;

	virtual const std::string& getFullName() = 0;

	virtual co::Module* getModule() = 0;

	virtual const std::string& getName() = 0;

	virtual co::Namespace* getParentNamespace() = 0;

	virtual co::ArrayRange<co::Type* const> getTypes() = 0;

	virtual co::Namespace* defineChildNamespace( const std::string& name ) = 0;

	virtual co::TypeBuilder* defineType( const std::string& name, co::TypeKind typeKind, co::TypeCreationTransaction* transaction ) = 0;

	virtual co::Namespace* getChildNamespace( const std::string& name ) = 0;

	virtual co::Type* getType( const std::string& name ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::Namespace> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::Namespace> { static const char* get() { return "co.Namespace"; } };
template<> struct typeOf<co::Namespace> : public typeOfBase<co::Namespace, InterfaceType> {};
} // namespace co

#endif // _CO_NAMESPACE_H_
