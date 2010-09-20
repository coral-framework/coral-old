/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPEDICTIONARYBUILDER_H_
#define _TYPEDICTIONARYBUILDER_H_

#include <co/ArrayRange.h>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace co {
	class Type;
	class EnumType;
	class StructType;
	class ComponentType;
	class InterfaceInfo;
	class InterfaceType;
	class MethodContainer;
	class NativeClassType;
	class AttributeContainer;
}

namespace ctemplate {
	class TemplateDictionary;
}

/*!
	Helper method that returns the name of the 'co' interface responsible for providing
	type introspection for a type (e.g. int32 -> "Type"; struct -> "StructType").
 */
const char* getTypeInterfaceName( co::Type* type );

/*!
	Helper class for building a ctemplate::TemplateDictionary with data concerning
	a co::Type instance (either for generating mappings or reflectors).
 */
class TypeDictionaryBuilder
{
public:
	//! Creates a ctemplate-dictionary-builder for the specified type.
	TypeDictionaryBuilder( co::Type* type );

	//! Destructor.
	~TypeDictionaryBuilder();

	/*!
		Builds a 'mapping' dictionary for the type.
	 */
	void buildMappingDict();

	//! Builds a 'reflector' dictionary for the type.
	void buildCodeDict();

	/*!
		Returns the dictionary built for the type by either of the buildXXX() methods.
		\warning The dictionary will be deleted when this TypeDictionaryBuilder instance dies.
	 */
	inline ctemplate::TemplateDictionary* getDictionary() const { return _dict; }

private:
	void fill();

	// Methods to fill in common data:
	void fillTypeData();
	void fillIncludes();
	void fillForwardDeclarations();
	void fillNestedForwardDecls();

	// Methods to fill in type-specific data:
	void fillEnumData( co::EnumType* enumType );
	void fillStructData( co::StructType* structType );
	void fillNativeClassData( co::NativeClassType* nativeClassType );
	void fillInterfaceData( co::InterfaceType* interfaceType );
	void fillInterfaceAncestor( co::InterfaceType* ancestor );
	void fillAttribAccessors( co::AttributeContainer* ac, ctemplate::TemplateDictionary* dict );
	void fillMethods( co::MethodContainer* mc, ctemplate::TemplateDictionary* dict );
	void fillComponentData( co::ComponentType* componentType );
	void fillInterfaceInfos( co::ArrayRange<co::InterfaceInfo* const> itfInfos, const char* sectionName );

	/*!
		Handles a reference for a type that's used by the current type.
		This either generates an '#include' or (preferably) a forward declaration.
	 */
	void includeType( co::Type* type );

	//! Adds a declaration to #include the specified header file.
	void includeHeader( const std::string& headerName );

	//! Adds a declaration to #include the given type's header file.
	void includeHeader( co::Type* type );

	//! Auxiliary method to include co::ArrayRange's header file.
	inline void includeArrayRangeHeader() { _includedArrayRange = true; }

	//! Auxiliary method to include co::RefPtr's header file.
	inline void includeRefPtrHeader() { _includedRefPtr = true; }

	//! Auxiliary method to include co::RefVector's header file.
	inline void includeRefVectorHeader() { _includedRefVector = true; }

	//! Auxiliary method to include std::vector's header file.
	inline void includeStdVectorHeader() { _includedStdVector = true; }

	//! Adds a forward declaration for the given type.
	void addForwardDeclaration( co::Type* type );

	//! Returns the given type's TypeKind as a string.
	std::string getKindName( co::Type* type );

	//! Formats a read-only cpp type name using the given type.
	//! Ex1: a return type 'MyInterface[]' is formatted as 'co::ArrayRange<MyInterface* const>'.
	//! Ex2: a type 'string' is formatted as 'const std::string &'.
	void formatInputType( co::Type* type, std::string& out );

	//! Formats a read-and-write cpp type name using the given type.
	//! Ex1: a return type 'MyInterface[]' is formatted as 'co::RefVector<MyInterface>&'.
	//! Ex2: a type 'string' is formatted as 'std::string &'.
	void formatOutputType( co::Type* type, std::string& out );

	// Formats a field type name used for struct members.
	//! Ex1: a return type 'MyInterface[]' is formatted as 'co::RefVector<MyInterface>'.
	//! Ex2: a type 'string' is formatted as 'std::string'.
	void formatFieldType( co::Type* type, std::string& out );

private:
	co::Type* _type;
	ctemplate::TemplateDictionary* _dict;

	enum OutputMode { Mode_Mapping, Mode_Code };
	OutputMode _mode;

	bool _includedArrayRange;
	bool _includedRefPtr;
	bool _includedRefVector;
	bool _includedStdVector;

	std::set<std::string> _includes;
	std::vector<co::Type*> _forwardDeclarations;
};

#endif // _TYPEDICTIONARYBUILDER_H_
