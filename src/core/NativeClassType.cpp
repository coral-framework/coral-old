/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "NativeClassType.h"

NativeClassType::~NativeClassType()
{
	// empty
}

void NativeClassType::setNativeName( const std::string& nativeName )
{
	_nativeName = nativeName;
}

void NativeClassType::setNativeHeaderFile( const std::string& headerName )
{
	_nativeHeaderFile = headerName;
}

const std::string& NativeClassType::getNativeName()
{
	return _nativeName;
}

const std::string& NativeClassType::getNativeHeaderFile()
{
	return _nativeHeaderFile;
}

CORAL_EXPORT_COMPONENT( NativeClassType, NativeClassTypeComponent );
