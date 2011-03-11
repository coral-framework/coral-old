/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_

#include "NamespaceComponent_Base.h"
#include <co/Type.h>
#include <co/Module.h>
#include <co/RefVector.h>

namespace co {

/*!
	Component that implements co.Namespace.
 */
class NamespaceComponent : public NamespaceComponent_Base
{
public:
	NamespaceComponent();
	virtual ~NamespaceComponent();
	
	// internal methods:
	void setParentAndName( Namespace* parent, const std::string& name );
	void setModule( Module* module );
	void addType( Type* type );
	void removeType( Type* type );
	
	// Namespace methods:
	const std::string& getName();
	const std::string& getFullName();
	Namespace* getParentNamespace();
	ArrayRange<Type* const> getTypes();
	ArrayRange<Namespace* const> getChildNamespaces();
	Module* getModule();
	Type* getType( const std::string& name );
	Namespace* getChildNamespace( const std::string& name );
	TypeBuilder* defineType( const std::string& name, TypeKind typeKind,
									TypeCreationTransaction* transaction );
	Namespace* defineChildNamespace( const std::string& name );

private:
	std::string getNamespaceName();
	void throwClashingType( const std::string& name );
	void throwClashingNamespace( const std::string& name );

private:
	std::string _name;
	std::string _fullName;
	Namespace* _parent;

	RefPtr<Module> _module;

	typedef RefVector<Type> TypeVector;
	TypeVector _types;
	
	typedef RefVector<Namespace> NamespaceVector;
	NamespaceVector _childNamespaces;
};

} // namespace co

#endif
