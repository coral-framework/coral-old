/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPEBUILDER_H_
#define _TYPEBUILDER_H_

#include "Namespace.h"
#include "TypeBuilder_Base.h"
#include <co/RefPtr.h>

namespace co {

// Forward Decls:
class TypeImpl;
class IField;
class IPort;
class IInterface;
class Method;

/*!
	Implements co.ITypeBuilder.
 */
class TypeBuilder : public TypeBuilder_Base
{
public:
	/*!
		Factory method.
		\pre kind >= TK_ENUM.
	 */
	static ITypeBuilder* create( TypeKind kind, INamespace* ns, const std::string& name );

public:
	virtual ~TypeBuilder();

	// Internal methods:

	// Called from IMethodBuilder to add a method definition.
	virtual void addMethod( Method* methodInfo );

	// Destroys and removes all references to our type, assuming it was not committed yet.
	void destroyType();

	// Executed once we're sure the type's transaction is going to be committed.
	virtual void commitType();

	// ITypeBuilder methods:
	INamespace* getNamespace();
	TypeKind getKind();
	const std::string& getTypeName();
	void defineIdentifier( const std::string& name );
	void defineField( const std::string& name, IType* type, bool isReadOnly );
	void defineBaseType( IType* superType );
	void definePort( const std::string& name, IInterface* type, bool isFacet );
	IMethodBuilder* defineMethod( const std::string& name );
	void defineNativeClass( const std::string& nativeHeader, const std::string& nativeName );
	IType* createType();

protected:
	TypeBuilder( TypeKind kind );

	//! Common initialization code, triggers the pre-allocation of our (still empty) type.
	void initialize( INamespace* ns, const std::string& name );

	//! Throws an exception if the type was already created (no further definitions allowed).
	void assertNotCreated();

	/*!
		Template method that guarantees the type to be construction by this builder is allocated.
		Returns true if the type was freshly allocated, or false if it was pre-allocated elsewhere
		(currently only 'co.IService' is pre-allocated).
	 */
	virtual bool allocateType() = 0;

	//! Template method to check for missing data (may throw MissingInputException).
	virtual void validate() = 0;

	//! Template method to populate the type with the data we have collected.
	//! Should only be called after validate() has made sure all required data is available.
	virtual void fillType() = 0;

protected:
	TypeKind _kind;
	RefPtr<Namespace> _namespace;
	std::string _name;

	RefPtr<IType> _type;
	bool _typeWasCreated;
};

} // namespace co

#endif
