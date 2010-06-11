/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPEMANAGER_H_
#define _TYPEMANAGER_H_

#include "Namespace.h"
#include "TypeManagerComponent_Base.h"
#include <co/RefPtr.h>

/*!
	Component that implements co.TypeManager.
 */
class TypeManager : public co::TypeManagerComponent_Base
{
public:
	TypeManager();
	virtual ~TypeManager();

	// internal methods:
	void initialize();

	// co::TypeManager methods:
	co::Namespace* getRootNS();
	co::Type* findType( const std::string& fullName );
	co::Namespace* findNamespace( const std::string& fullName );
	co::Type* getType( const std::string& typeName );
	co::ArrayType* getArrayOf( co::Type* elementType );
	co::Type* loadType( const std::string& typeName, std::vector<co::CSLError>* errorStack );

private:
	co::Type* loadTypeOrThrow( const std::string& fullName );

	co::ArrayType* defineArrayType( co::Type* elementType );

	void definePrimitiveType( Namespace* ns, const std::string& name, co::TypeKind kind );
	void defineBuiltInTypes();

private:
	co::RefPtr<Namespace> _rootNS;
};

#endif
