/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IRECORDTYPE_H_
#define _CO_IRECORDTYPE_H_

#include <co/TypeTraits.h>
#include <co/ICompositeType.h>
#include <co/Range.h>

// Forward Declarations:
namespace co {
	class IField;
} // namespace co
// End Of Forward Declarations

// co.IRecordType Mapping:
namespace co {

class IRecordType : public co::ICompositeType
{
public:
	virtual ~IRecordType() {;}

	virtual co::Range<co::IField* const> getFields() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IRecordType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IRecordType> { static const char* get() { return "co.IRecordType"; } };
} // namespace co

#endif // _CO_IRECORDTYPE_H_
