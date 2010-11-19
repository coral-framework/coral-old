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
	void addDocumentation( const std::string& typeOrMemberName, const std::string& text );
	void addCppBlock( const std::string& interfaceName, const std::string& text );
	const std::string& getCppBlock( const std::string& interfaceName );

	// co::TypeManager methods:
	co::Namespace* getRootNS();
	bool getDocumentationParsing();
	void setDocumentationParsing( bool documentationParsing );
	co::Type* findType( const std::string& fullName );
	co::Namespace* findNamespace( const std::string& fullName );
	co::Type* getType( const std::string& typeName );
	co::ArrayType* getArrayOf( co::Type* elementType );
	co::Type* loadType( const std::string& typeName, std::vector<co::CSLError>& errorStack );
	const std::string& getDocumentation( const std::string& typeOrMemberName );

private:
	co::Type* loadTypeOrThrow( const std::string& fullName );

	co::ArrayType* defineArrayType( co::Type* elementType );

	void definePrimitiveType( Namespace* ns, const std::string& name, co::TypeKind kind );
	void defineBuiltInTypes();

private:
	co::RefPtr<Namespace> _rootNS;

	bool _docParsing;

	typedef std::map<std::string, std::string> DocMap;
	DocMap _docMap;

	typedef std::map<std::string, std::string> CppBlockMap;
	CppBlockMap _cppBlockMap;
};

#endif
