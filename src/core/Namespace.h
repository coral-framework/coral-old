/*
 * Coral - A lightweight C++ component framework
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

private:
	std::string _name;
	std::string _fullName;
	INamespace* _parent;

	RefPtr<IModule> _module;

	typedef RefVector<IType> TypeVector;
	TypeVector _types;
	
	typedef RefVector<INamespace> NamespaceVector;
	NamespaceVector _childNamespaces;
};

} // namespace co

#endif
