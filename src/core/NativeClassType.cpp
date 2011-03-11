/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "NativeClassType.h"

namespace co {

NativeClassTypeComponent::~NativeClassTypeComponent()
{
	// empty
}

void NativeClassTypeComponent::setNativeName( const std::string& nativeName )
{
	_nativeName = nativeName;
}

void NativeClassTypeComponent::setNativeHeaderFile( const std::string& headerName )
{
	_nativeHeaderFile = headerName;
}

const std::string& NativeClassTypeComponent::getNativeName()
{
	return _nativeName;
}

const std::string& NativeClassTypeComponent::getNativeHeaderFile()
{
	return _nativeHeaderFile;
}

CORAL_EXPORT_COMPONENT( NativeClassTypeComponent, NativeClassTypeComponent );

} // namespace co
