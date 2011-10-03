/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPEMANAGER_H_
#define _CO_TYPEMANAGER_H_

#include "Namespace.h"
#include "TypeManager_Base.h"
#include <co/RefPtr.h>

namespace co {

/*!
	Implements co.ITypeManager.
 */
class TypeManager : public TypeManager_Base
{
public:
	TypeManager();
	virtual ~TypeManager();

	// internal methods:
	void initialize();

	// ITypeManager methods:
	INamespace* getRootNS();
	IType* findType( const std::string& fullName );
	INamespace* findNamespace( const std::string& fullName );
	IType* getType( const std::string& typeName );
	IArray* getArrayOf( IType* elementType );
	IType* loadType( const std::string& typeName, std::vector<CSLError>& errorStack );

private:
	IType* loadTypeOrThrow( const std::string& fullName );

	IArray* defineArrayType( IType* elementType );

	void definePrimitiveType( Namespace* ns, const std::string& name, TypeKind kind );
	void defineBuiltInTypes();

private:
	RefPtr<Namespace> _rootNS;
};

} // namespace co

#endif
