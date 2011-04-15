/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_BASICREFLECTOR_H_
#define _CO_BASICREFLECTOR_H_

#include "reserved/ReflectorBase.h"

namespace co {

/*!
	Reflector for all "basic" types (i.e. any, all primitives, arrays
	and enums). Also doubles as a reflector for all "internal" components
	(that can't have a real reflector, such as the reflectors themselves).
 */
class BasicReflector : public ReflectorBase
{
public:
	/*!
		Factory method.

		\pre \a type must be either of a non-user-definable kind,
				or an internal component.
	 */
	static IReflector* create( IType* type );

public:
	// IReflector methods:
	IType* getType();
	uint32 getSize() = 0;

protected:
	inline BasicReflector( IType* t ) : _type( t ) {;}

	IType* _type;
};

} // namespace co

#endif
