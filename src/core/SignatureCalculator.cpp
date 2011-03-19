/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "SignatureCalculator.h"
#include <co/Uuid.h>
#include <co/IEnumType.h>
#include <co/IArrayType.h>
#include <co/IMethodInfo.h>
#include <co/IStructType.h>
#include <co/ITypeManager.h>
#include <co/IInterfaceType.h>
#include <co/IAttributeInfo.h>
#include <co/IParameterInfo.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/INativeClassType.h>

/*
	Changing this constant will automatically change ALL signatures.
	Use this to signal type incompatibilities in new Coral versions.
 */
const co::uint8 SIGNATURE_ALGORITHM_VERSION = 1;

// A byte to clearly delimiter an element and avoid ambiguities
const co::uint8 DELIMITER = ';';

SignatureCalculator::SignatureCalculator( co::IType* type ) : _type( type ),
	_fullSignatureHash( co::CryptoHash::Sha1 ), _binarySignatureHash( co::CryptoHash::Sha1 )
{
	assert( _type );
}

SignatureCalculator::~SignatureCalculator()
{
	// empty
}

void SignatureCalculator::calculate()
{
	calculateSignatures();
}

inline void getUuidFromHash( co::CryptoHash& hash, co::Uuid& uuid )
{
	co::CryptoHash::Result result;
	hash.getResult( result );
	result.toUuid( uuid );
}

void SignatureCalculator::calculateSignatures()
{
	addDataToSignatures( SIGNATURE_ALGORITHM_VERSION );

	const co::uint8 kind = _type->getKind();
	addDataToSignatures( kind );

	_fullSignatureHash.addData( _type->getFullName() );

	switch( _type->getKind() )
	{
	case co::TK_ANY:
	case co::TK_BOOLEAN:
	case co::TK_INT8:
	case co::TK_UINT8:
	case co::TK_INT16:
	case co::TK_UINT16:
	case co::TK_INT32:
	case co::TK_UINT32:
	case co::TK_INT64:
	case co::TK_UINT64:
	case co::TK_FLOAT:
	case co::TK_DOUBLE:
	case co::TK_STRING:
		// empty
		break;
	case co::TK_ARRAY:
		fillArraySignatureData();
		break;
	case co::TK_ENUM:
		fillEnumSignatureData();
		break;
	case co::TK_EXCEPTION:
		// empty
		break;
	case co::TK_STRUCT:
		fillStructSignatureData();
		break;
	case co::TK_NATIVECLASS:
		fillNativeClassSignatureData();
		break;
	case co::TK_INTERFACE:
		fillInterfaceSignatureData();
		break;
	case co::TK_COMPONENT:
		fillComponentSignatureData();
		break;
	default:
		assert( false );
	}

	getUuidFromHash( _fullSignatureHash, _fullSignature );
	getUuidFromHash( _binarySignatureHash, _binarySignature );
}

void SignatureCalculator::fillArraySignatureData()
{
	co::IArrayType* array = static_cast<co::IArrayType*>( _type );
	co::IType* elementType = array->getElementType();
	_fullSignatureHash.addData( elementType->getFullSignature() );
	_binarySignatureHash.addData( elementType->getBinarySignature() );
}

void SignatureCalculator::fillEnumSignatureData()
{
	co::IEnumType* enumType = static_cast<co::IEnumType*>( _type );
	co::ArrayRange<std::string const> range = enumType->getIdentifiers();
	for( ; range; range.popFirst() )
	{
		addDataToSignatures( range.getFirst() );

		// add a delimiter to avoid ambiguities
		addDataToSignatures( DELIMITER );
	}
}

void SignatureCalculator::fillStructSignatureData()
{
	co::IStructType* structType = static_cast<co::IStructType*>( _type );
	co::ArrayRange<co::IAttributeInfo* const> fields = structType->getMemberAttributes();
	for( ; fields; fields.popFirst() )
	{
		co::IAttributeInfo* ai = fields.getFirst();
		assert( !ai->getIsReadOnly() );

		co::IType* type = ai->getType();
		_fullSignatureHash.addData( type->getFullSignature() );
		_binarySignatureHash.addData( type->getBinarySignature() );

		_fullSignatureHash.addData( ai->getName() );
	}
}

void SignatureCalculator::fillNativeClassSignatureData()
{
	co::INativeClassType* nativeType = static_cast<co::INativeClassType*>( _type );
	_fullSignatureHash.addData( nativeType->getNativeName() );
	_fullSignatureHash.addData( nativeType->getNativeHeaderFile() );
	fillAttributeAndMethodContainerData();
}

void SignatureCalculator::fillInterfaceSignatureData()
{
	co::IInterfaceType* interfaceType = static_cast<co::IInterfaceType*>( _type );

	_binarySignatureHash.addData( _type->getFullName() );

	// add all inherited signatures
	co::ArrayRange<co::IInterfaceType* const> superTypes = interfaceType->getSuperInterfaces();
	for( ; superTypes; superTypes.popFirst() )
	{
		co::IInterfaceType* super = superTypes.getFirst();
		_fullSignatureHash.addData( super->getFullSignature() );
		_binarySignatureHash.addData( super->getBinarySignature() );
	}

	fillAttributeAndMethodContainerData();
}

void SignatureCalculator::fillComponentSignatureData()
{
	co::IComponentType* componentType = static_cast<co::IComponentType*>( _type );

	co::ArrayRange<co::IInterfaceInfo* const> interfaces = componentType->getInterfaces();
	for( ; interfaces; interfaces.popFirst() )
	{
		co::IInterfaceInfo* itf = interfaces.getFirst();

		const co::uint8 isFacet = itf->getIsFacet() ? 1 : 0;
		addDataToSignatures( isFacet );

		co::IType* type = itf->getType();
		_fullSignatureHash.addData( type->getFullSignature() );
		_binarySignatureHash.addData( type->getBinarySignature() );

		addDataToSignatures( itf->getName() );
	}
}

void SignatureCalculator::fillAttributeAndMethodContainerData()
{
	co::IAttributeContainer* attributeContainer = dynamic_cast<co::IAttributeContainer*>( _type );
	assert( attributeContainer );

	co::ArrayRange<co::IAttributeInfo* const> attributes = attributeContainer->getMemberAttributes();
	for( ; attributes; attributes.popFirst() )
	{
		co::IAttributeInfo* ai = attributes.getFirst();

		const co::uint8 isReadOnly = ai->getIsReadOnly() ? 1 : 0;
		addDataToSignatures( isReadOnly );

		addTypeWithNoSignature( ai->getType() );
		addDataToSignatures( ai->getName() );
	}

	co::IMethodContainer* methodContainer = dynamic_cast<co::IMethodContainer*>( _type );
	assert( methodContainer );

	co::ArrayRange<co::IMethodInfo* const> methods = methodContainer->getMemberMethods();
	for( ; methods; methods.popFirst() )
	{
		co::IMethodInfo* mi = methods.getFirst();

		co::IType* returnType = mi->getReturnType();
		if( returnType )
			addTypeWithNoSignature( returnType );
		else
			addDataToSignatures( co::Uuid::null() );

		addDataToSignatures( mi->getName() );

		co::ArrayRange<co::IParameterInfo* const> parameters = mi->getParameters();
		for( ; parameters; parameters.popFirst() )
		{
			co::IParameterInfo* pi = parameters.getFirst();

			const co::uint8 isIn = pi->getIsIn() ? 1 : 0;
			addDataToSignatures( isIn );

			const co::uint8 isOut = pi->getIsOut() ? 1 : 0;
			addDataToSignatures( isOut );

			addTypeWithNoSignature( pi->getType() );

			_fullSignatureHash.addData( pi->getName() );
		}
	}
}

void SignatureCalculator::addTypeWithNoSignature( co::IType* type )
{
	const co::uint8 kind = type->getKind();
	addDataToSignatures( kind );
	addDataToSignatures( type->getFullName() );
}
