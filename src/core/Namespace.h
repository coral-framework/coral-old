/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_

#include "Namespace_Base.h"
#include <co/IType.h>
#include <co/IModule.h>
#include <co/RefVector.h>

namespace co {

/*!
	Implements co.INamespace.
 */
class Namespace : public Namespace_Base
{
public:
	Namespace();
	virtual ~Namespace();
	
	// internal methods:
	void setParentAndName( INamespace* parent, const std::string& name );
	void setModule( IModule* module );
	void addType( IType* type );
	void removeType( IType* type );
	
	// INamespace methods:
	const std::string& getName();
	const std::string& getFullName();
	INamespace* getParentNamespace();
	Range<IType* const> getTypes();
	Range<INamespace* const> getChildNamespaces();
	IModule* getModule();
	IType* getType( const std::string& name );
	INamespace* getChildNamespace( const std::string& name );
	ITypeBuilder* defineType( const std::string& name, TypeKind typeKind,
									ITypeTransaction* transaction );
	INamespace* defineChildNamespace( const std::string& name );

private:
	std::string getNamespaceName();
	void throwClashingType( const std::string& name );
	void throwClashingNamespace( const std::string& name );

	inline static int typeComparator( const std::string& name, IType* type )
	{
		return name.compare( type->getName() );
	}

	inline IType* findType( const std::string& name )
	{
		size_t pos;
		if( binarySearch( Range<IType* const>( _types ), name, typeComparator, pos ) )
			return _types[pos].get();
		return NULL;
	}

	inline static int namespaceComparator( const std::string& name, INamespace* ns )
	{
		return name.compare( ns->getName() );
	}

	inline INamespace* findChildNamespace( const std::string& name )
	{
		size_t pos;
		if( binarySearch( Range<INamespace* const>( _childNamespaces ), name, namespaceComparator, pos ) )
			return _childNamespaces[pos].get();
		return NULL;
	}

private:
	std::string _name;
	std::string _fullName;
	INamespace* _parent;
	RefPtr<IModule> _module;
	RefVector<IType> _types;
	RefVector<INamespace> _childNamespaces;
};

} // namespace co

#endif
