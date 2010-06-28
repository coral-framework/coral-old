/*******************************************************************************
** C++ mapping generated for type 'co.TypeManager'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_TYPEMANAGER_H_
#define _CO_TYPEMANAGER_H_

#include <co/TypeTraits.h>
#include <co/Interface.h>
#include <vector>

// Forward Declarations:
namespace co {
	class ArrayType;
	class Namespace;
	class Type;
	struct CSLError;
} // namespace co
// End Of Forward Declarations

// co.TypeManager Mapping:
namespace co {

class TypeManager : public co::Interface
{
public:
	virtual ~TypeManager() {;}

	virtual co::Namespace* getRootNS() = 0;

	virtual co::Namespace* findNamespace( const std::string& fullName ) = 0;

	virtual co::Type* findType( const std::string& fullName ) = 0;

	virtual co::ArrayType* getArrayOf( co::Type* elementType ) = 0;

	virtual co::Type* getType( const std::string& typeName ) = 0;

	virtual co::Type* loadType( const std::string& typeName, std::vector<co::CSLError>* errorStack ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::TypeManager> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::TypeManager> { static const char* get() { return "co.TypeManager"; } };
template<> struct typeOf<co::TypeManager> : public typeOfBase<co::TypeManager, InterfaceType> {};
} // namespace co

#endif // _CO_TYPEMANAGER_H_
