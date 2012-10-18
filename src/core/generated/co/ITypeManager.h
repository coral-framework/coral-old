/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ITYPEMANAGER_H_
#define _CO_ITYPEMANAGER_H_

#include <co/TypeTraits.h>
#include <co/Range.h>
#include <vector>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	struct CSLError;
	class IArray;
	class INamespace;
	class IType;
	class ITypeTransaction;
} // namespace co
// End Of Forward Declarations

// co.ITypeManager Mapping:
namespace co {

class ITypeManager : public co::IService
{
public:
	virtual ~ITypeManager() {;}

	virtual co::INamespace* getRootNS() = 0;

	virtual co::ITypeTransaction* getTransaction() = 0;

	virtual co::INamespace* findNamespace( const std::string& fullName ) = 0;

	virtual co::IType* findType( const std::string& fullName ) = 0;

	virtual co::IArray* getArrayOf( co::IType* elementType ) = 0;

	virtual co::INamespace* getNamespace( const std::string& fullName ) = 0;

	virtual co::IType* getType( const std::string& typeName ) = 0;

	virtual co::IType* loadType( const std::string& typeName, std::vector<co::CSLError>& errorStack ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ITypeManager> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ITypeManager> { static const char* get() { return "co.ITypeManager"; } };
} // namespace co

#endif // _CO_ITYPEMANAGER_H_
