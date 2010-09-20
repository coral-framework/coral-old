/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPEBUILDER_H_
#define _TYPEBUILDER_H_

#include "TypeBuilderComponent_Base.h"
#include <co/RefPtr.h>

class TypeImpl;

namespace co {
	class MethodInfo;
	class AttributeInfo;
	class InterfaceInfo;
	class InterfaceType;
}

/*!
	Component that implements co.TypeBuilder.
 */
class TypeBuilder : public co::TypeBuilderComponent_Base
{
public:
	/*!
		Factory method.
		\pre kind >= co::TK_ENUM.
	 */
	static TypeBuilder* create( co::TypeKind kind, co::Namespace* ns, const std::string& name );

public:
	virtual ~TypeBuilder();

	/*!
		Internal method. Destroys and removes all references to the type created by this builder.
		This only works if the type was not committed, and is used in TypeCreationTransaction::rollback().
	 */
	void destroyType();

	//! Template method: tries to add a method definition (called from MethodBuilder).
	virtual void addMethod( co::MethodInfo* methodInfo );

	// co::TypeBuilder methods:
	co::Namespace* getNamespace();
	co::TypeKind getKind();
	const std::string& getTypeName();
	void defineIdentifier( const std::string& name );
	void defineAttribute( const std::string& name, co::Type* type, bool isReadOnly );
	void defineSuperType( co::Type* superType );
	void defineInterface( const std::string& name, co::InterfaceType* interface, bool isProvided );
	co::MethodBuilder* defineMethod( const std::string& name );
	void defineNativeClass( const std::string& nativeHeaderFile, const std::string& nativeName );
	co::Type* createType();

protected:
	TypeBuilder( co::TypeKind kind );

	//! Common initialization code, triggers the pre-allocation of our (still empty) type.
	void initialize( co::Namespace* ns, const std::string& name );

	//! Throws an exception if the type was already created (no further definitions allowed).
	void assertNotCreated();

	/*!
		Template method that guarantees the type to be construction by this builder is allocated.
		Returns true if the type was freshly allocated, or false if it was pre-allocated elsewhere
		(currently only 'co.Interface' is pre-allocated).
	 */
	virtual bool allocateType() = 0;

	//! Template method to check for missing data (may throw co::MissingInputException).
	virtual void validate() = 0;

	//! Template method to populate the type with the data we have collected.
	//! Should only be called after validate() has made sure all required data is available.
	virtual void fillType() = 0;

protected:
	co::TypeKind _kind;
	co::RefPtr<co::Namespace> _namespace;
	std::string _name;

	co::RefPtr<co::Type> _type;
	bool _typeWasCreated;
};

#endif
