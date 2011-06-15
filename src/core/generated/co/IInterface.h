/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IINTERFACE_H_
#define _CO_IINTERFACE_H_

#include <co/TypeTraits.h>
#include <co/IClassType.h>
#include <co/IInterface.h>
#include <co/Range.h>

// co.IInterface Mapping:
namespace co {

class IInterface : public co::IClassType
{
public:
	virtual ~IInterface() {;}

	virtual co::IInterface* getBaseType() = 0;

	virtual const std::string& getCppBlock() = 0;

	virtual co::Range<co::IInterface* const> getSubTypes() = 0;

	virtual co::Range<co::IInterface* const> getSuperTypes() = 0;

	virtual bool isSubTypeOf( co::IInterface* type ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IInterface> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IInterface> { static const char* get() { return "co.IInterface"; } };
template<> struct typeOf<co::IInterface> : public typeOfBase<co::IInterface, IInterface> {};
} // namespace co

#endif // _CO_IINTERFACE_H_
