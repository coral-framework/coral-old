/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ICPPBLOCK_H_
#define _CO_ICPPBLOCK_H_

#include <co/TypeTraits.h>
#include <co/IAnnotation.h>

// co.ICppBlock Mapping:
namespace co {

class ICppBlock : public co::IAnnotation
{
public:
	virtual ~ICppBlock() {;}

	virtual const std::string& getValue() = 0;

	virtual void setValue( const std::string& value ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ICppBlock> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ICppBlock> { static const char* get() { return "co.ICppBlock"; } };
template<> struct typeOf<co::ICppBlock> : public typeOfBase<co::ICppBlock, IInterface> {};
} // namespace co

#endif // _CO_ICPPBLOCK_H_
