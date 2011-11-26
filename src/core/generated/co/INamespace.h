/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INAMESPACE_H_
#define _CO_INAMESPACE_H_

#include <co/TypeTraits.h>
#include <co/Range.h>
#include <co/INamespace.h>
#include <co/TypeKind.h>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IModule;
	class IType;
	class ITypeBuilder;
} // namespace co
// End Of Forward Declarations

// co.INamespace Mapping:
namespace co {

class INamespace : public co::IService
{
public:
	virtual ~INamespace() {;}

	virtual co::Range<co::INamespace* const> getChildNamespaces() = 0;

	virtual const std::string& getFullName() = 0;

	virtual co::IModule* getModule() = 0;

	virtual const std::string& getName() = 0;

	virtual co::INamespace* getParentNamespace() = 0;

	virtual co::Range<co::IType* const> getTypes() = 0;

	virtual co::INamespace* defineChildNamespace( const std::string& name ) = 0;

	virtual co::ITypeBuilder* defineType( const std::string& name, co::TypeKind typeKind ) = 0;

	virtual co::INamespace* getChildNamespace( const std::string& name ) = 0;

	virtual co::IType* getType( const std::string& name ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::INamespace> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::INamespace> { static const char* get() { return "co.INamespace"; } };
} // namespace co

#endif // _CO_INAMESPACE_H_
