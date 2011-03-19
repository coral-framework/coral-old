/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _NATIVECLASSTYPE_H_
#define _NATIVECLASSTYPE_H_

#include "Type.h"
#include "AttributeAndMethodContainer.h"
#include "NativeClassType_Base.h"

namespace co {

/*!
	Implements co.INativeClassType.
 */
class NativeClassType : public NativeClassType_Base,
						public TypeImpl, public AttributeAndMethodContainer
{
public:
	virtual ~NativeClassType();

	// internal methods:
	void setNativeName( const std::string& nativeName );
	void setNativeHeaderFile( const std::string& nativeHeaderFile );

	// INativeClassType methods:
	const std::string& getNativeName();
	const std::string& getNativeHeaderFile();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
	DELEGATE_CO_COMPOUNDTYPE_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_ATTRIBUTECONTAINER_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_METHODCONTAINER_METHODS( AttributeAndMethodContainer:: );

private:
	std::string _nativeName;
	std::string _nativeHeaderFile;
};

} // namespace co

#endif
