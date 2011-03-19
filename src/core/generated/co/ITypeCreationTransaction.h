/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ITYPECREATIONTRANSACTION_H_
#define _CO_ITYPECREATIONTRANSACTION_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class ITypeBuilder;
} // namespace co
// End Of Forward Declarations

// co.ITypeCreationTransaction Mapping:
namespace co {

class ITypeCreationTransaction : public co::Interface
{
public:
	virtual ~ITypeCreationTransaction() {;}

	virtual co::ArrayRange<co::ITypeBuilder* const> getTypeBuilders() = 0;

	virtual void commit() = 0;

	virtual void rollback() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ITypeCreationTransaction> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ITypeCreationTransaction> { static const char* get() { return "co.ITypeCreationTransaction"; } };
template<> struct typeOf<co::ITypeCreationTransaction> : public typeOfBase<co::ITypeCreationTransaction, IInterfaceType> {};
} // namespace co

#endif // _CO_ITYPECREATIONTRANSACTION_H_
