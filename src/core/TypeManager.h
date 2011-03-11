/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPEMANAGER_H_
#define _TYPEMANAGER_H_

#include "Namespace.h"
#include "TypeManagerComponent_Base.h"
#include <co/RefPtr.h>
#include <map>

namespace co {

/*!
	Component that implements co.TypeManager.
 */
class TypeManagerComponent : public TypeManagerComponent_Base
{
public:
	TypeManagerComponent();
	virtual ~TypeManagerComponent();

	// internal methods:
	void initialize();
	void addDocumentation( const std::string& typeOrMemberName, const std::string& text );
	void addCppBlock( const std::string& interfaceName, const std::string& text );
	const std::string& getCppBlock( const std::string& interfaceName );

	// TypeManager methods:
	Namespace* getRootNS();
	bool getDocumentationParsing();
	void setDocumentationParsing( bool documentationParsing );
	Type* findType( const std::string& fullName );
	Namespace* findNamespace( const std::string& fullName );
	Type* getType( const std::string& typeName );
	ArrayType* getArrayOf( Type* elementType );
	Type* loadType( const std::string& typeName, std::vector<CSLError>& errorStack );
	const std::string& getDocumentation( const std::string& typeOrMemberName );

private:
	Type* loadTypeOrThrow( const std::string& fullName );

	ArrayType* defineArrayType( Type* elementType );

	void definePrimitiveType( NamespaceComponent* ns, const std::string& name, TypeKind kind );
	void defineBuiltInTypes();

private:
	RefPtr<NamespaceComponent> _rootNS;

	bool _docParsing;

	typedef std::map<std::string, std::string> DocMap;
	DocMap _docMap;

	typedef std::map<std::string, std::string> CppBlockMap;
	CppBlockMap _cppBlockMap;
};

} // namespace co

#endif
