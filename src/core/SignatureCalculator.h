/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _SIGNATURECALCULATOR_H_
#define _SIGNATURECALCULATOR_H_

#include <co/Platform.h>
#include "tools/CryptoHash.h"

namespace co {
	class IType;
	class IMethodInfo;
} // namespace co

/*!
	Calculates version hashes for binary or full type compatibility checking.

	The \b binary signature keeps track of changes that break binary compatibility between
	versions of a C++ type mapping, while the \b full signature also keeps track of changes
	that could break source (or reflective) code compatibility (e.g. renaming struct fields).

	\par References:
		- http://techbase.kde.org/Policies/Binary_Compatibility_Issues_With_C++
		- http://techbase.kde.org/Policies/Binary_Compatibility_Examples
		- http://labs.trolltech.com/blogs/2009/08/12/some-thoughts-on-binary-compatibility/

	Implementation Details:
		- Cannot raise exceptions.
		- Signatures are computed on demand within the type components.
		- Based on SHA-1 hashing of the data available through the public type API.
		- A type's signature cannot depend on another type's signature, except in places
			where mutual dependencies would never occur.
 */
class SignatureCalculator
{
public:
	//! Type for whom we should calculate, and on which we should set, the signatures.
	SignatureCalculator( co::IType* type );

	//! Destructor.
	~SignatureCalculator();

	//! Calculates and sets the type's signatures.
	void calculate();

	//! Returns the calculated full signature.
	inline const co::Uuid& getFullSignature() const { return _fullSignature; }

	//! Returns the calculated binary signature.
	inline const co::Uuid& getBinarySignature() const { return _binarySignature; }

private:
	template<typename T>
	inline void addDataToSignatures( const T& data )
	{
		_fullSignatureHash.addData( data );
		_binarySignatureHash.addData( data );
	}

	void calculateSignatures();

	void fillArraySignatureData();
	void fillEnumSignatureData();
	void fillStructSignatureData();
	void fillNativeClassSignatureData();
	void fillInterfaceSignatureData();
	void fillComponentSignatureData();
	void fillAttributeAndMethodContainerData();

	/*!
		Adds just enough data about a type. For places where using the
		type's signature would imply in mutual dependencies (i.e. method
		parameters and return types).
	 */
	void addTypeWithNoSignature( co::IType* type );

private:
	co::IType* _type;

	co::Uuid _fullSignature;
	co::Uuid _binarySignature;

	co::CryptoHash _fullSignatureHash;
	co::CryptoHash _binarySignatureHash;
};

#endif // _SIGNATURECALCULATOR_H_
