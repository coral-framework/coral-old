/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "NativeClass.h"

namespace co {

NativeClass::~NativeClass()
{
	// empty
}

void NativeClass::setNativeName( const std::string& nativeName )
{
	_nativeName = nativeName;
}

void NativeClass::setNativeHeader( const std::string& headerName )
{
	_nativeHeaderFile = headerName;
}

const std::string& NativeClass::getNativeName()
{
	return _nativeName;
}

const std::string& NativeClass::getNativeHeader()
{
	return _nativeHeaderFile;
}

CORAL_EXPORT_COMPONENT( NativeClass, NativeClass );

} // namespace co
