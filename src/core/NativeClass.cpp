/*
 * Coral - A lightweight C++ component framework
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

void NativeClass::setNativeHeaderFile( const std::string& headerName )
{
	_nativeHeaderFile = headerName;
}

const std::string& NativeClass::getNativeName()
{
	return _nativeName;
}

const std::string& NativeClass::getNativeHeaderFile()
{
	return _nativeHeaderFile;
}

CORAL_EXPORT_COMPONENT( NativeClass, NativeClass );

} // namespace co
