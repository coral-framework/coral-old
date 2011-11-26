/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ICLASSTYPE_H_
#define _CO_ICLASSTYPE_H_

#include <co/TypeTraits.h>
#include <co/Range.h>
#include <co/IRecordType.h>

// Forward Declarations:
namespace co {
	class IMethod;
} // namespace co
// End Of Forward Declarations

// co.IClassType Mapping:
namespace co {

class IClassType : public co::IRecordType
{
public:
	virtual ~IClassType() {;}

	virtual co::Range<co::IMethod* const> getMethods() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IClassType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IClassType> { static const char* get() { return "co.IClassType"; } };
} // namespace co

#endif // _CO_ICLASSTYPE_H_
