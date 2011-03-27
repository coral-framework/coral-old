/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include "Type_Base.h"
#include <co/Uuid.h>
#include <co/RefPtr.h>
#include <co/INamespace.h>
#include <co/IReflector.h>

namespace co {

/*!
	Re-usable implementation of IType.
 */
class TypeImpl
{
public:
	TypeImpl();

   	// internal methods:
	void setType( INamespace* parent, const std::string& name, TypeKind kind );

	// IType methods:
	const std::string& getName();
	const std::string& getFullName();
	INamespace* getNamespace();
	TypeKind getKind();
	const Uuid& getFullSignature( IType* myType );
	const Uuid& getBinarySignature( IType* myType );
	IReflector* getReflector( IType* myType );
	void setReflector( IReflector* reflector );

private:
	void calculateSignatures( IType* myType );

private:
	INamespace* _namespace;
	std::string _name;
	std::string _fullName;
	TypeKind _kind;

	bool _hasSignatures;			// whether _fullSignature and _binarySignature are valid
	bool _isCalculatingSignatures;	// to detect cycles in the signatures algorithm

	Uuid _fullSignature;
	Uuid _binarySignature;

	RefPtr<IReflector> _reflector;
};

#define DELEGATE_co_IType( DELEGATE ) \
	virtual const std::string& getName() { return DELEGATE getName(); } \
	virtual const std::string& getFullName() { return DELEGATE getFullName(); } \
	virtual INamespace* getNamespace() { return DELEGATE getNamespace(); } \
	virtual TypeKind getKind() { return DELEGATE getKind(); } \
	virtual const Uuid& getFullSignature() { return DELEGATE getFullSignature( this ); } \
	virtual const Uuid& getBinarySignature() { return DELEGATE getBinarySignature( this ); } \
	virtual IReflector* getReflector() { return DELEGATE getReflector( this ); } \
	virtual void setReflector( IReflector* reflector ) { DELEGATE setReflector( reflector ); } \

/*!
	Implements co.IType.
 */
class Type : public Type_Base, public TypeImpl
{
public:
	virtual ~Type();

	DELEGATE_co_IType( TypeImpl:: );
};

} // namespace co

#endif
