/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include "Type_Base.h"
#include <co/RefVector.h>
#include <co/INamespace.h>
#include <co/IReflector.h>
#include <co/IAnnotation.h>
#include <co/reserved/Uuid.h>

namespace co {

//! Selects an annotation by type, considering generalizations.
IAnnotation* selectAnnotation( Range<IAnnotation*> annotations, IInterface* requestedType );

/*!
	Re-usable implementation of co::IAnnotated.
 */
template<class Base>
class Annotated : public Base
{
public:
	Range<IAnnotation*> getAnnotations()
	{
		return _annotations;
	}

	void setAnnotations( Range<IAnnotation*> annotations )
	{
		return co::assign( annotations, _annotations );
	}

	inline void addAnnotation( IAnnotation* annotation )
	{
		_annotations.push_back( annotation );
	}

	IAnnotation* getAnnotation( IInterface* requestedType )
	{
		return selectAnnotation( _annotations, requestedType );
	}

private:
	RefVector<IAnnotation> _annotations;
};

/*!
	Re-usable implementation of IType.
 */
class TypeImpl
{
public:
	inline TypeImpl() : _namespace( NULL ), _kind( TK_NONE ),
		_hasSignatures( false ), _isCalculatingSignatures( false )
	{;}

	void setType( INamespace* parent, const std::string& name, TypeKind kind );

	IReflector* getReflector( IType* myType );
	void setReflector( IReflector* reflector );

protected:
	void calculateSignatures( IType* myType );

protected:
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

template<class Base>
class Type : public Annotated<Base>, public TypeImpl
{
public:
	std::string getName() { return _name; }
	std::string getFullName() { return _fullName; }
	INamespace* getNamespace() { return _namespace; }
	TypeKind getKind() { return _kind; }

	Uuid getFullSignature()
	{
		if( !_hasSignatures )
			calculateSignatures( this );
		return _fullSignature;
	}
	
	Uuid getBinarySignature()
	{
		if( !_hasSignatures )
			calculateSignatures( this );
		return _binarySignature;
	}

	IReflector* getReflector() { return TypeImpl::getReflector( this ); }
	void setReflector( IReflector* reflector ) { _reflector = reflector; }

	IReflector* getCurrentReflector() { return _reflector.get(); }

	bool isA( IType* type ) { return this == type; }
};

/*!
	Implements co.Type.
 */
class TypeComponent : public Type<Type_Base>
{
public:
	virtual ~TypeComponent();
};

} // namespace co

#endif
