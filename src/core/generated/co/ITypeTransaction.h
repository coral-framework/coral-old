/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ITYPETRANSACTION_H_
#define _CO_ITYPETRANSACTION_H_

#include <co/TypeTraits.h>
#include <co/Range.h>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class ITypeBuilder;
} // namespace co
// End Of Forward Declarations

// co.ITypeTransaction Mapping:
namespace co {

class ITypeTransaction : public co::IService
{
public:
	virtual ~ITypeTransaction() {;}

	virtual co::Range<co::ITypeBuilder*> getTypeBuilders() = 0;

	virtual void commit() = 0;

	virtual void rollback() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ITypeTransaction> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ITypeTransaction> { static const char* get() { return "co.ITypeTransaction"; } };
} // namespace co

#endif // _CO_ITYPETRANSACTION_H_
