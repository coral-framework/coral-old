/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_

#include "NamespaceComponent_Base.h"
#include <co/Type.h>
#include <co/Module.h>
#include <co/RefVector.h>

/*!
	Component that implements co.Namespace.
 */
class Namespace : public co::NamespaceComponent_Base
{
public:
	Namespace();
	virtual ~Namespace();
	
	// internal methods:
	void setParentAndName( co::Namespace* parent, const std::string& name );
	void setModule( co::Module* module );
	void addType( co::Type* type );
	void removeType( co::Type* type );
	
	// co::Namespace methods:
	const std::string& getName();
	const std::string& getFullName();
	co::Namespace* getParentNamespace();
	co::ArrayRange<co::Type* const> getTypes();
	co::ArrayRange<co::Namespace* const> getChildNamespaces();
	co::Module* getModule();
	co::Type* getType( const std::string& name );
	co::Namespace* getChildNamespace( const std::string& name );
	co::TypeBuilder* defineType( const std::string& name, co::TypeKind typeKind,
									co::TypeCreationTransaction* transaction );
	co::Namespace* defineChildNamespace( const std::string& name );

private:
	std::string getNamespaceName();
	void throwClashingType( const std::string& name );
	void throwClashingNamespace( const std::string& name );

private:
	std::string _name;
	std::string _fullName;
	co::Namespace* _parent;

	co::RefPtr<co::Module> _module;

	typedef co::RefVector<co::Type> TypeVector;
	TypeVector _types;
	
	typedef co::RefVector<co::Namespace> NamespaceVector;
	NamespaceVector _childNamespaces;
};

#endif
