/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPEMANAGER_H_
#define _TYPEMANAGER_H_

#include "Namespace.h"
#include "TypeManager_Base.h"
#include <co/RefPtr.h>
#include <map>

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
	void addDocumentation( const std::string& typeOrMemberName, const std::string& text );
	void addCppBlock( const std::string& interfaceName, const std::string& text );
	const std::string& getCppBlock( const std::string& interfaceName );

	// ITypeManager methods:
	INamespace* getRootNS();
	bool getDocumentationParsing();
	void setDocumentationParsing( bool documentationParsing );
	IType* findType( const std::string& fullName );
	INamespace* findNamespace( const std::string& fullName );
	IType* getType( const std::string& typeName );
	IArray* getArrayOf( IType* elementType );
	IType* loadType( const std::string& typeName, std::vector<CSLError>& errorStack );
	const std::string& getDocumentation( const std::string& typeOrMemberName );

private:
	IType* loadTypeOrThrow( const std::string& fullName );

	IArray* defineArrayType( IType* elementType );

	void definePrimitiveType( Namespace* ns, const std::string& name, TypeKind kind );
	void defineBuiltInTypes();

private:
	RefPtr<Namespace> _rootNS;

	bool _docParsing;

	typedef std::map<std::string, std::string> DocMap;
	DocMap _docMap;

	typedef std::map<std::string, std::string> CppBlockMap;
	CppBlockMap _cppBlockMap;
};

} // namespace co

#endif
