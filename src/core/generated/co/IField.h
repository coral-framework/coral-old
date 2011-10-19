/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IFIELD_H_
#define _CO_IFIELD_H_

#include <co/TypeTraits.h>
#include <co/IMember.h>

// Forward Declarations:
namespace co {
	class IType;
} // namespace co
// End Of Forward Declarations

// co.IField Mapping:
namespace co {

class IField : public co::IMember
{
public:
	virtual ~IField() {;}

	virtual bool getIsReadOnly() = 0;

	virtual co::IType* getType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IField> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IField> { static const char* get() { return "co.IField"; } };
} // namespace co

#endif // _CO_IFIELD_H_
