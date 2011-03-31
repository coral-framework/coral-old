/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPESEMANTICCHECKER_H_
#define _CO_TYPESEMANTICCHECKER_H_

#include <set>
#include <string>

namespace co {

class IType;
class ICompositeType;
class IField;
class IInterface;

/*!
	Performs several checks to assure a type is semantically consistent.
 */
class TypeSemanticChecker
{
public:
	//! The passed \a type and it's inheritance tree should be fully loaded.
	TypeSemanticChecker( IType* type );

	//! Performs the semantic analysis of the type.
	//! Throws a co::SemanticException in case of errors.
	void check();

private:
	enum MemberType
	{
		Field,
		FieldGetter,
		FieldSetter,
		Method
	};

	class MemberDeclaration
	{
	public:
		inline co::IType* getDeclaringType() const { return _declaringType; }
		inline MemberType getMemberType() const { return _memberType; }
		inline const std::string& getMemberName() const { return _memberName; }

		MemberDeclaration( MemberType memberType, const std::string& memberName, co::IType* declaringType )
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
		co::IType* _declaringType;
	};

	// stream output operator support for MemberDeclaration
	friend std::ostream& operator<<( std::ostream&, const co::TypeSemanticChecker::MemberDeclaration& );

	// validates inheritance: checks for cyclic inheritance and multiple inheritance.
	void checkInheritance( co::IInterface* interface );

	// validates inherited members: checks for clashing names between the checked type and its super-types
	void checkInheritedMemberClashing();

	// checks if the member declaration clashes with the previously declared and inserts it if not.
	void insertMemberDeclaration( const MemberDeclaration& memberDeclaration );

	// inserts the field declarations - member name and accessors.
	void insertFieldDeclaration( co::IField* field, co::IType* declaringType );

	// checks the type members declarations.
	// If available checks for the fields and methods possible clashes, and checks superTypes if they exist.
	void checkMemberDeclarations( co::IType* type );

private:
	IType* _type;

	typedef std::set<MemberDeclaration> MemberSet;
	MemberSet _memberDeclarations;

	typedef std::set<IType*> TypeSet;
	TypeSet _visitedTypes;
};

} // namespace co

#endif // _CO_TYPESEMANTICCHECKER_H_
