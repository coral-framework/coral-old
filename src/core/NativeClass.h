/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_NATIVECLASS_H_
#define _CO_NATIVECLASS_H_

#include "CompositeType.h"
#include "NativeClass_Base.h"

namespace co {

/*!
	Implements co.INativeClass.
 */
class NativeClass : public NativeClass_Base, public CompositeTypeImpl
{
public:
	virtual ~NativeClass();

	// internal methods:
	void setNativeName( const std::string& nativeName );
	void setNativeHeaderFile( const std::string& nativeHeaderFile );

	// INativeClass methods:
	const std::string& getNativeName();
	const std::string& getNativeHeaderFile();

	DELEGATE_co_IType( CompositeTypeImpl:: );
	DELEGATE_co_ICompositeType( CompositeTypeImpl:: );
	DELEGATE_co_IRecordType( CompositeTypeImpl:: );
	DELEGATE_co_IClassType( CompositeTypeImpl:: );

private:
	std::string _nativeName;
	std::string _nativeHeaderFile;
};

} // namespace co

#endif
