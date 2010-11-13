/*
 * Coral - Lightweight C++ Component Framework.
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
	BasicReflector( co::Type* type );
	virtual ~BasicReflector();

	// co::Reflector methods:
	co::Type* getType();
	co::int32 getSize();
	Component* newInstance();

private:
	co::Type* _type;
};

#endif // _BASICREFLECTOR_H_
