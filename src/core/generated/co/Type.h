/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPE_H_
#define _CO_TYPE_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>
#include <co/TypeKind.h>

// Forward Declarations:
namespace co {
	class Namespace;
	class Reflector;
	class Uuid;
} // namespace co
// End Of Forward Declarations

// co.Type Mapping:
namespace co {

class Type : public co::Interface
{
public:
	virtual ~Type() {;}

	virtual const co::Uuid& getBinarySignature() = 0;

	virtual const std::string& getFullName() = 0;

	virtual const co::Uuid& getFullSignature() = 0;

	virtual co::TypeKind getKind() = 0;

	virtual const std::string& getName() = 0;

	virtual co::Namespace* getNamespace() = 0;

	virtual co::Reflector* getReflector() = 0;

	virtual void setReflector( co::Reflector* reflector ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::Type> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::Type> { static const char* get() { return "co.Type"; } };
template<> struct typeOf<co::Type> : public typeOfBase<co::Type, InterfaceType> {};
} // namespace co

#endif // _CO_TYPE_H_
