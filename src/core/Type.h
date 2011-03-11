/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include "TypeComponent_Base.h"
#include <co/Uuid.h>
#include <co/RefPtr.h>
#include <co/Namespace.h>
#include <co/Reflector.h>

namespace co {

/*!
	Re-usable implementation of Type.
 */
class TypeImpl
{
public:
	TypeImpl();

   	// internal methods:
	void setType( Namespace* parent, const std::string& name, TypeKind kind );

	// Type methods:
	const std::string& getName();
	const std::string& getFullName();
	Namespace* getNamespace();
	TypeKind getKind();
	const Uuid& getFullSignature( Type* myType );
	const Uuid& getBinarySignature( Type* myType );
	Reflector* getReflector( Type* myType );
	void setReflector( Reflector* reflector );

private:
	void calculateSignatures( Type* myType );

private:
	Namespace* _namespace;
	std::string _name;
	std::string _fullName;
	TypeKind _kind;

	bool _hasSignatures;			// whether _fullSignature and _binarySignature are valid
	bool _isCalculatingSignatures;	// to detect cycles in the signatures algorithm

	Uuid _fullSignature;
	Uuid _binarySignature;

	RefPtr<Reflector> _reflector;
};

#define DELEGATE_CO_TYPE_METHODS( DELEGATE ) \
	virtual const std::string& getName() { return DELEGATE getName(); } \
	virtual const std::string& getFullName() { return DELEGATE getFullName(); } \
	virtual Namespace* getNamespace() { return DELEGATE getNamespace(); } \
	virtual TypeKind getKind() { return DELEGATE getKind(); } \
	virtual const Uuid& getFullSignature() { return DELEGATE getFullSignature( this ); } \
	virtual const Uuid& getBinarySignature() { return DELEGATE getBinarySignature( this ); } \
	virtual Reflector* getReflector() { return DELEGATE getReflector( this ); } \
	virtual void setReflector( Reflector* reflector ) { DELEGATE setReflector( reflector ); } \

/*!
	Component that implements co.Type.
 */
class TypeComponent : public TypeComponent_Base, public TypeImpl
{
public:
	virtual ~TypeComponent();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
};

} // namespace co

#endif
