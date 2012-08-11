/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IPARAMETER_H_
#define _CO_IPARAMETER_H_

#include <co/TypeTraits.h>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IType;
} // namespace co
// End Of Forward Declarations

// co.IParameter Mapping:
namespace co {

class IParameter : public co::IService
{
public:
	virtual ~IParameter() {;}

	virtual bool getIsIn() = 0;

	virtual bool getIsOut() = 0;

	virtual std::string getName() = 0;

	virtual co::IType* getType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IParameter> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IParameter> { static const char* get() { return "co.IParameter"; } };
} // namespace co

#endif // _CO_IPARAMETER_H_
