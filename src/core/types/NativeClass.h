/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_NATIVECLASS_H_
#define _CO_NATIVECLASS_H_

#include "ClassType.h"
#include "NativeClass_Base.h"

namespace co {

/*!
	Implements co.INativeClass.
 */
class NativeClass : public ClassType<NativeClass_Base>
{
public:
	virtual ~NativeClass();

	// internal methods:
	void setNativeName( const std::string& nativeName );
	void setNativeHeader( const std::string& nativeHeaderFile );

	// INativeClass methods:
	const std::string& getNativeName();
	const std::string& getNativeHeader();

private:
	std::string _nativeName;
	std::string _nativeHeaderFile;
};

} // namespace co

#endif
