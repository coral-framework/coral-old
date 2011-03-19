/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ITYPEMANAGER_H_
#define _CO_ITYPEMANAGER_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <vector>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	struct CSLError;
	class IArrayType;
	class INamespace;
	class IType;
} // namespace co
// End Of Forward Declarations

// co.ITypeManager Mapping:
namespace co {

class ITypeManager : public co::Interface
{
public:
	virtual ~ITypeManager() {;}

	virtual bool getDocumentationParsing() = 0;

	virtual void setDocumentationParsing( bool documentationParsing ) = 0;

	virtual co::INamespace* getRootNS() = 0;

	virtual co::INamespace* findNamespace( const std::string& fullName ) = 0;

	virtual co::IType* findType( const std::string& fullName ) = 0;

	virtual co::IArrayType* getArrayOf( co::IType* elementType ) = 0;

	virtual const std::string& getDocumentation( const std::string& typeOrMemberName ) = 0;

	virtual co::IType* getType( const std::string& typeName ) = 0;

	virtual co::IType* loadType( const std::string& typeName, std::vector<co::CSLError>& errorStack ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ITypeManager> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ITypeManager> { static const char* get() { return "co.ITypeManager"; } };
template<> struct typeOf<co::ITypeManager> : public typeOfBase<co::ITypeManager, IInterfaceType> {};
} // namespace co

#endif // _CO_ITYPEMANAGER_H_
