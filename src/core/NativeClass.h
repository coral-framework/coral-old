/*
 * Coral - A lightweight C++ component framework
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
class NativeClass : public NativeClass_Base, public ClassTypeImpl
{
public:
	virtual ~NativeClass();

	// internal methods:
	void setNativeName( const std::string& nativeName );
	void setNativeHeader( const std::string& nativeHeaderFile );

	// INativeClass methods:
	const std::string& getNativeName();
	const std::string& getNativeHeader();

	DELEGATE_co_IType( ClassTypeImpl:: );
	DELEGATE_co_ICompositeType( ClassTypeImpl:: );
	DELEGATE_co_IRecordType( ClassTypeImpl:: );
	DELEGATE_co_IClassType( ClassTypeImpl:: );

private:
	std::string _nativeName;
	std::string _nativeHeaderFile;
};

} // namespace co

#endif
