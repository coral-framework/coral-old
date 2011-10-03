/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ITYPE_H_
#define _CO_ITYPE_H_

#include <co/TypeTraits.h>
#include <co/IAnnotated.h>
#include <co/Uuid.h>
#include <co/TypeKind.h>

// Forward Declarations:
namespace co {
	class INamespace;
	class IReflector;
} // namespace co
// End Of Forward Declarations

// co.IType Mapping:
namespace co {

class IType : public co::IAnnotated
{
public:
	virtual ~IType() {;}

	virtual const co::Uuid& getBinarySignature() = 0;

	virtual const std::string& getFullName() = 0;

	virtual const co::Uuid& getFullSignature() = 0;

	virtual co::TypeKind getKind() = 0;

	virtual const std::string& getName() = 0;

	virtual co::INamespace* getNamespace() = 0;

	virtual co::IReflector* getReflector() = 0;

	virtual void setReflector( co::IReflector* reflector ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IType> { static const char* get() { return "co.IType"; } };
template<> struct typeOf<co::IType> : public typeOfBase<co::IType, IInterface> {};
} // namespace co

#endif // _CO_ITYPE_H_
