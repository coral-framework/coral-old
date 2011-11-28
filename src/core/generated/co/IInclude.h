/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IINCLUDE_H_
#define _CO_IINCLUDE_H_

#include <co/TypeTraits.h>
#include <co/IAnnotation.h>

// co.IInclude Mapping:
namespace co {

class IInclude : public co::IAnnotation
{
public:
	virtual ~IInclude() {;}

	virtual const std::string& getValue() = 0;

	virtual void setValue( const std::string& value ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IInclude> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IInclude> { static const char* get() { return "co.IInclude"; } };
} // namespace co

#endif // _CO_IINCLUDE_H_
