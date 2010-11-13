/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include "TypeComponent_Base.h"
#include <co/Uuid.h>
#include <co/RefPtr.h>
#include <co/Namespace.h>
#include <co/Reflector.h>

/*!
	Re-usable implementation of co::Type.
 */
class TypeImpl
{
public:
	TypeImpl();

   	// internal methods:
	void setType( co::Namespace* parent, const std::string& name, co::TypeKind kind );

	// co::Type methods:
	const std::string& getName();
	const std::string& getFullName();
	co::Namespace* getNamespace();
	co::TypeKind getKind();
	const co::Uuid& getFullSignature( co::Type* myType );
	const co::Uuid& getBinarySignature( co::Type* myType );
	co::Reflector* getReflector( co::Type* myType );
	void setReflector( co::Reflector* reflector );

private:
	void calculateSignatures( co::Type* myType );

private:
	co::Namespace* _namespace;
	std::string _name;
	std::string _fullName;
	co::TypeKind _kind;

	bool _hasSignatures;			// whether _fullSignature and _binarySignature are valid
	bool _isCalculatingSignatures;	// to detect cycles in the signatures algorithm

	co::Uuid _fullSignature;
	co::Uuid _binarySignature;

	co::RefPtr<co::Reflector> _reflector;
};

#define DELEGATE_CO_TYPE_METHODS( DELEGATE ) \
	virtual const std::string& getName() { return DELEGATE getName(); } \
	virtual const std::string& getFullName() { return DELEGATE getFullName(); } \
	virtual co::Namespace* getNamespace() { return DELEGATE getNamespace(); } \
	virtual co::TypeKind getKind() { return DELEGATE getKind(); } \
	virtual const co::Uuid& getFullSignature() { return DELEGATE getFullSignature( this ); } \
	virtual const co::Uuid& getBinarySignature() { return DELEGATE getBinarySignature( this ); } \
	virtual co::Reflector* getReflector() { return DELEGATE getReflector( this ); } \
	virtual void setReflector( co::Reflector* reflector ) { DELEGATE setReflector( reflector ); } \

/*!
	Component that implements co.Type.
 */
class Type : public co::TypeComponent_Base, public TypeImpl
{
public:
	virtual ~Type();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
};

#endif
