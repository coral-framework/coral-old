/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPESEMANTICCHECKER_H_
#define _CO_TYPESEMANTICCHECKER_H_

#include <set>
#include <string>

namespace co {

class Type;
class CompoundType;
class AttributeInfo;
class InterfaceType;

/*!
	Performs several checks to assure a type is semantically consistent.
 */
class TypeSemanticChecker
{
public:
	//! The passed \a type and it's inheritance tree should be fully loaded.
	TypeSemanticChecker( Type* type );

	//! Performs the semantic analysis of the type.
	//! Throws a co::SemanticException in case of errors.
	void check();

private:
	enum MemberType
	{
		Attribute,
		GetterMethod,
		SetterMethod,
		Method
	};
	
	class MemberDeclaration
	{
	public:
		inline co::Type* getDeclaringType() const { return _declaringType; }
		inline MemberType getMemberType() const { return _memberType; }
		inline const std::string& getMemberName() const { return _memberName; }
		
		MemberDeclaration( MemberType memberType, const std::string& memberName, co::Type* declaringType )
			: _memberType( memberType ), _memberName( memberName ), _declaringType( declaringType )
		{;}

		bool operator<( const MemberDeclaration& member ) const
		{
			return member._memberName < _memberName;
		}
		
		bool operator==( const MemberDeclaration& member ) const
		{
			return member._memberName == _memberName;
		}

	private:
		MemberType _memberType;
		const std::string _memberName;
		co::Type* _declaringType;
	};

	// stream output operator support for MemberDeclaration
	friend std::ostream& operator<<( std::ostream&, const co::TypeSemanticChecker::MemberDeclaration& );

	// validates inheritance: checks for cyclic inheritance and multiple inheritance.
	void checkInheritance( co::InterfaceType* interfaceType );

	// validates inherited members: checks for clashing names between the checked type and its super-types
	void checkInheritedMemberClashing();

	// checks if the member declaration clashes with the previously declared and inserts it if not.
	void insertMemberDeclaration( const MemberDeclaration& memberDeclaration );

	// inserts de attribute declarations - member name and accessors.
	void insertAttributeDeclaration( co::AttributeInfo* attribute, co::Type* declaringType );

	// checks the type members declarations.
	// If avilable checks for the attributes and methods possible clashes, and checks superTypes if they exist.
	void checkMemberDeclarations( co::Type* type );

private:
	Type* _type;

	typedef std::set<MemberDeclaration> MemberSet;
	MemberSet _memberDeclarations;

	typedef std::set<Type*> TypeSet;
	TypeSet _visitedTypes;
};

} // namespace co

#endif // _CO_TYPESEMANTICCHECKER_H_
