/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ISTRUCT_H_
#define _CO_ISTRUCT_H_

#include <co/TypeTraits.h>
#include <co/IRecordType.h>

// co.IStruct Mapping:
namespace co {

class IStruct : public co::IRecordType
{
public:
	virtual ~IStruct() {;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::IStruct> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IStruct> { static const char* get() { return "co.IStruct"; } };
template<> struct typeOf<co::IStruct> : public typeOfBase<co::IStruct, IInterface> {};
} // namespace co

#endif // _CO_ISTRUCT_H_
