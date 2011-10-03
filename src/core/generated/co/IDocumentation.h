/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IDOCUMENTATION_H_
#define _CO_IDOCUMENTATION_H_

#include <co/TypeTraits.h>
#include <co/IAnnotation.h>

// co.IDocumentation Mapping:
namespace co {

class IDocumentation : public co::IAnnotation
{
public:
	virtual ~IDocumentation() {;}

	virtual const std::string& getValue() = 0;

	virtual void setValue( const std::string& value ) = 0;

	virtual void addDocFor( const std::string& element, const std::string& text ) = 0;

	virtual const std::string& getDocFor( const std::string& element ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IDocumentation> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IDocumentation> { static const char* get() { return "co.IDocumentation"; } };
template<> struct typeOf<co::IDocumentation> : public typeOfBase<co::IDocumentation, IInterface> {};
} // namespace co

#endif // _CO_IDOCUMENTATION_H_
