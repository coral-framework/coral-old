/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _BASICREFLECTOR_H_
#define _BASICREFLECTOR_H_

#include "reserved/ReflectorBase.h"

/*!
	The reflector for all "basic" types; i.e. any, all primitives, arrays,
	enums and exceptions. Also doubles as a reflector for all the "internal"
	components (i.e. lower-level components that don't have a functioning
	reflector, such as the reflectors themselves).
 */
class BasicReflector : public co::ReflectorBase
{
public:
	BasicReflector( co::IType* type );
	virtual ~BasicReflector();

	// co::IReflector methods:
	co::IType* getType();
	co::int32 getSize();
	IComponent* newInstance();

private:
	co::IType* _type;
};

#endif // _BASICREFLECTOR_H_
