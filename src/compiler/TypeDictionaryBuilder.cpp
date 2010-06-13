/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TypeDictionaryBuilder.h"
#include "HelperFunctions.h"

#include <co/Any.h>
#include <co/Type.h>
#include <co/Uuid.h>
#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/System.h>
#include <co/EnumType.h>
#include <co/ArrayType.h>
#include <co/Namespace.h>
#include <co/RefVector.h>
#include <co/MethodInfo.h>
#include <co/StructType.h>
#include <co/TypeManager.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>
#include <co/AttributeInfo.h>
#include <co/ParameterInfo.h>
#include <co/MethodContainer.h>
#include <co/NativeClassType.h>
#include <co/AttributeContainer.h>
#include <co/reserved/LexicalUtilities.h>
#include <core/tools/StringTokenizer.h>
#include <ctemplate/template.h>
#include <algorithm>


TypeDictionaryBuilder::TypeDictionaryBuilder( co::Type* type )
{
	assert( type );
	assert( type->getKind() > co::TK_ARRAY );

	_type = type;
	_dict = NULL;

	_includedArrayRange = false;
	_includedRefPtr = false;
	_includedRefVector = false;
	_includedStdVector = false;
}

TypeDictionaryBuilder::~TypeDictionaryBuilder()
{
	delete _dict;
}

void TypeDictionaryBuilder::buildMappingDict( const std::map<std::string, std::string>& cppBlockMap )
{
	assert( _dict == NULL );

	_dict = new ctemplate::TemplateDictionary( "type_mapping" );

	_mode = Mode_Mapping;

	fill();

	// add C++ block data, if available
	if( _type->getKind() != co::TK_INTERFACE )
		return;

	std::map<std::string, std::string>::const_iterator it = cppBlockMap.find( _type->getFullName() );
	if( it == cppBlockMap.end() )
		return;

	const std::string& code = it->second;
	if( code.empty() )
		return;

	ctemplate::TemplateDictionary* cppBlock = _dict->AddIncludeDictionary( "CPP_BLOCK" );
	cppBlock->SetFilename( "cpp_block.tpl" );
	cppBlock->SetValue( "CODE", code );
}

void TypeDictionaryBuilder::buildCodeDict()
{
	assert( _dict == NULL );

	_dict = new ctemplate::TemplateDictionary( "type_code" );

	_mode = Mode_Code;

	// weather we're generating a reflector for the core ('co') module
	_dict->ShowSection( _type->getNamespace()->getFullName() == "co" ?
							"MODULE_IS_CO_TRUE" : "MODULE_IS_CO_FALSE" );

	fill();
}

void TypeDictionaryBuilder::fill()
{
	fillTypeData();

	switch( _type->getKind() )
	{
	case co::TK_ENUM:
		fillEnumData( dynamic_cast<co::EnumType*>( _type ) );
		break;

	case co::TK_EXCEPTION:
		includeHeader( "co/UserException.h" );
		break;

	case co::TK_STRUCT:
		fillStructData( dynamic_cast<co::StructType*>( _type ) );
		break;

	case co::TK_NATIVECLASS:
		fillNativeClassData( dynamic_cast<co::NativeClassType*>( _type ) );
		break;

	case co::TK_INTERFACE:
		fillInterfaceData( dynamic_cast<co::InterfaceType*>( _type ) );
		break;

	case co::TK_COMPONENT:
		fillComponentData( dynamic_cast<co::ComponentType*>( _type ) );
		break;

	default:
		assert( false );
	}

	fillIncludes();
	fillForwardDeclarations();
}

void TypeDictionaryBuilder::fillTypeData()
{
	// compute the 'full upperscore' name
	std::string upperScore( _type->getFullName() );
	convertSeparators( upperScore, '.', '_' );
	std::transform( upperScore.begin(), upperScore.end(), upperScore.begin(), ::toupper );

	_dict->SetValue( "TYPE_NAME", _type->getName() );
	_dict->SetValue( "TYPE_NAME_CPP", getCppName( _type ) );
	_dict->SetValue( "TYPE_NAME_FULL", _type->getFullName() );
	_dict->SetValue( "TYPE_NAME_FULL_UPPERSCORE", upperScore );
	_dict->SetValue( "TYPE_KIND", getKindName( _type ) );

	if( _mode == Mode_Code )
	{
		// compute the type's "file path"
		std::string filePath( _type->getFullName() );
		convertSeparators( filePath, '.', '/' );
		_dict->SetValue( "TYPE_FILE_PATH", filePath );
	}

	_dict->SetValue( "TYPE_TYPE_CLASS", getTypeInterfaceName( _type ) );

	co::StringTokenizer st( _type->getNamespace()->getFullName(), "." );
	while( st.nextToken() )
	{
		ctemplate::TemplateDictionary* subDict = _dict->AddSectionDictionary( "NAMESPACE_LIST" );
		subDict->SetValue( "NAMESPACE", st.getToken() );
	}

	_dict->SetValue( "NS", getCppName( _type->getNamespace()->getFullName() ) );
}

void TypeDictionaryBuilder::fillIncludes()
{
	if( _includedRefVector )
		includeHeader( "co/RefVector.h" );
	else
	{
		if( _includedArrayRange )
			includeHeader( "co/ArrayRange.h" );
		else if( _includedStdVector )
			includeHeader( "vector" );

		if( _includedRefPtr )
			includeHeader( "co/RefPtr.h" );
	}

	for( std::set<std::string>::iterator it = _includes.begin(); it != _includes.end(); ++it )
	{
		ctemplate::TemplateDictionary* headerList = _dict->AddSectionDictionary( "HEADER_LIST" );
		headerList->SetValue( "HEADER", *it );
	}
}

bool sortTypesByNamespace( co::Type* a, co::Type* b )
{
	if( a == b )
		return false;

	co::Namespace* nsA = a->getNamespace();
	co::Namespace* nsB = b->getNamespace();

	if( nsA == nsB )
		return a->getName() < b->getName();

	return nsA->getFullName() < nsB->getFullName();
}

struct NamespaceAndDict
{
	NamespaceAndDict()
	{;}

	NamespaceAndDict( co::Namespace* ns, ctemplate::TemplateDictionary* dict )
		: ns( ns ), dict( dict )
	{;}

	co::Namespace* ns;
	ctemplate::TemplateDictionary* dict;
};

void TypeDictionaryBuilder::fillForwardDeclarations()
{
	if( _forwardDeclarations.empty() )
		return;

	if( _mode == Mode_Mapping )
	{
		_dict->SetValue( "BEGIN_FORWARD_DECLARATIONS", "\n// Forward Declarations:\n" );
		_dict->SetValue( "END_FORWARD_DECLARATIONS", "\n// End Of Forward Declarations\n" );
	}

	// sort the _forwardDeclarations vector by namespace (full name), then type name
	std::sort( _forwardDeclarations.begin(), _forwardDeclarations.end(), sortTypesByNamespace );

	// remove duplicates
	_forwardDeclarations.erase(
		std::unique( _forwardDeclarations.begin(), _forwardDeclarations.end() ),
		_forwardDeclarations.end()
	);

	if( _mode == Mode_Mapping )
	{
		fillNestedForwardDecls();
	}
	else
	{
		// reflectors must actually include forward-declared types
		std::string filename;
		std::size_t count = _forwardDeclarations.size();
		for( std::size_t i = 0; i < count; ++i )
		{
			ctemplate::TemplateDictionary* headerList = _dict->AddSectionDictionary( "FW_DECL_LIST" );
			filename = _forwardDeclarations[i]->getFullName();
			typeToHeaderName( filename );
			headerList->SetValue( "HEADER", filename );
		}
	}
}

void TypeDictionaryBuilder::fillNestedForwardDecls()
{
	// create template dictionaries for the 'forest of namespaces', with the class/struct declarations inside
	std::vector<NamespaceAndDict> forwardDeclStack;
	forwardDeclStack.push_back( NamespaceAndDict( co::getSystem()->getTypes()->getRootNS(), _dict ) );

	std::vector<co::Namespace*> typeNamespaces;

	std::size_t count = _forwardDeclarations.size();
	for( std::size_t i = 0; i < count; ++i )
	{
		co::Type* type = _forwardDeclarations[i];
		co::Namespace* ns = type->getNamespace();

		NamespaceAndDict* currentFrame = &forwardDeclStack.back();

		// if we're in the wrong namespace, we gotta pop/push stuff from/to the forwardDeclStack
		if( ns != currentFrame->ns )
		{
			// gather the type's namespace hierarchy into a stack
			assert( typeNamespaces.empty() );
			co::Namespace* currentNS = ns;
			while( currentNS )
			{
				typeNamespaces.push_back( currentNS );
				currentNS = currentNS->getParentNamespace();
			}

			// both root namespaces must match
			assert( forwardDeclStack[0].ns == typeNamespaces.back() );

			// skip the common namespace levels
			std::size_t commonLevels = 1;
			for( std::size_t k = typeNamespaces.size() - 2; k > 0; --k )
			{
				if( k >= count || typeNamespaces[k] != forwardDeclStack[commonLevels].ns )
					break;
				++commonLevels;
			}

			// forwardDeclStack.pop_back() until we reach the common level
			while( commonLevels > forwardDeclStack.size() )
			{
				forwardDeclStack.pop_back();
				currentFrame = &forwardDeclStack.back();
			}

			assert( commonLevels == forwardDeclStack.size() );

			// create any necessary sub namespaces
			while( commonLevels < typeNamespaces.size() )
			{
				co::Namespace* subNS = typeNamespaces[typeNamespaces.size() - commonLevels - 1];

				ctemplate::TemplateDictionary* subDecls = currentFrame->dict->AddIncludeDictionary( "FORWARD_DECLARATIONS" );
				subDecls->SetFilename( "forward_decls.tpl" );
				subDecls->SetValue( "NAMESPACE", subNS->getName() );

				forwardDeclStack.push_back( NamespaceAndDict( subNS, subDecls ) );
				currentFrame = &forwardDeclStack.back();
				++commonLevels;
			}

			typeNamespaces.clear();
		}

		// now we're in the correct namespace, just add the type declaration
		assert( ns == currentFrame->ns );

		const char* sectionName = ( type->getKind() == co::TK_STRUCT ? "STRUCT_LIST" : "CLASS_LIST" );
		const char* valueName = ( type->getKind() == co::TK_STRUCT ? "STRUCT_NAME" : "CLASS_NAME" );
		ctemplate::TemplateDictionary* section = currentFrame->dict->AddSectionDictionary( sectionName );
		section->SetValue( valueName, type->getName() );
	}
}

void TypeDictionaryBuilder::fillEnumData( co::EnumType* enumType )
{
	co::ArrayRange<std::string const> identifiers = enumType->getIdentifiers();
	for( ; identifiers; identifiers.popFirst() )
	{
		ctemplate::TemplateDictionary* subDict = _dict->AddSectionDictionary( "IDENTIFIER_LIST" );
		subDict->SetValue( "IDENTIFIER", identifiers.getFirst() );
	}
}

struct AttributeSize
{
	co::AttributeInfo* ai;
	co::uint32 size;

	AttributeSize( co::AttributeInfo* ai, co::uint32 size )
		: ai( ai ), size( size ) {;}

	inline bool operator<( const AttributeSize& other ) const
	{
		return size > other.size;
	}
};

co::uint32 estimateFieldSize( co::AttributeInfo* ai )
{
	co::Type* type = ai->getType();

	co::uint32 res = 0;
	switch( type->getKind() )
	{
	case co::TK_ANY:		res = sizeof(co::Any); break;
	case co::TK_BOOLEAN:	res = 1; break;
	case co::TK_INT8:
	case co::TK_UINT8:		res = 1; break;
	case co::TK_INT16:
	case co::TK_UINT16:		res = 2; break;
	case co::TK_INT32:
	case co::TK_UINT32:		res = 4; break;
	case co::TK_INT64:
	case co::TK_UINT64:		res = 8; break;
	case co::TK_FLOAT:		res = sizeof(float); break;
	case co::TK_DOUBLE:		res = sizeof(double); break;
	case co::TK_STRING:		res = sizeof(std::string); break;
	case co::TK_ARRAY:		res = sizeof(co::RefVector<co::Interface>); break;
	case co::TK_ENUM:		res = sizeof(co::uint32); break;
	case co::TK_STRUCT:
		{
			co::StructType* structType = dynamic_cast<co::StructType*>( type );
			assert( structType );
			co::ArrayRange<co::AttributeInfo* const> attributes = structType->getMemberAttributes();
			for( ; attributes; attributes.popFirst() )
				res += estimateFieldSize( attributes.getFirst() );
		}
		break;
	case co::TK_NATIVECLASS:res = 4 * sizeof(double); break; // average estimate
	case co::TK_INTERFACE:	res = sizeof(co::RefPtr<co::Interface>); break;
	default:
		assert( false );
	}

	return res;
}

void TypeDictionaryBuilder::fillStructData( co::StructType* structType )
{
	co::ArrayRange<co::AttributeInfo* const> attributes = structType->getMemberAttributes();

	if( _mode == Mode_Mapping )
	{
		std::size_t fieldCount = attributes.getSize();

		// first we must estimate the sizes of the struct's fields
		std::vector<AttributeSize> fields;
		fields.reserve( fieldCount );
		for( ; attributes; attributes.popFirst() )
		{
			co::AttributeInfo* ai = attributes.getFirst();
			fields.push_back( AttributeSize( ai, estimateFieldSize( ai ) ) );
		}

		// sort by field size (descending)
		std::sort( fields.begin(), fields.end() );

		std::string typeName;
		for( std::size_t i = 0; i < fieldCount; ++i )
		{
			const AttributeSize& field = fields[i];

			includeType( field.ai->getType() );

			ctemplate::TemplateDictionary* attributeDict = _dict->AddSectionDictionary( "ATTRIBUTE_LIST" );

			formatFieldType( field.ai->getType(), typeName );
			attributeDict->SetValue( "TYPE", typeName );
			attributeDict->SetValue( "NAME", field.ai->getName() );
			attributeDict->SetIntValue( "SIZE", field.size );
		}
	}
	else
	{
		std::string typeName;
		for( ; attributes; attributes.popFirst() )
		{
			co::AttributeInfo* ai = attributes.getFirst();

			co::Type* type = ai->getType();
			includeType( type );

			ctemplate::TemplateDictionary* attributeDict = _dict->AddSectionDictionary( "ATTRIBUTE_LIST" );
	
			attributeDict->SetValue( "NAME", ai->getName() );
			attributeDict->SetIntValue( "INDEX", ai->getIndex() );
	
			formatInputType( type, typeName );
			attributeDict->SetValue( "TYPE", typeName );

			if( type->getKind() == co::TK_ARRAY )
			{
				co::ArrayType* at = dynamic_cast<co::ArrayType*>( type );
				assert( at );
				formatInputType( at->getElementType(), typeName );
				attributeDict->SetValue( "ELEMENT_TYPE", typeName );
			}

			attributeDict->ShowSection( type->getKind() == co::TK_ARRAY ? "IS_ARRAY_TRUE" : "IS_ARRAY_FALSE" );
		

			if( type->getKind() == co::TK_INTERFACE )
				attributeDict->ShowSection( "NEED_GET" );
		}
	}
}

void TypeDictionaryBuilder::fillNativeClassData( co::NativeClassType* nativeClassType )
{
	const std::string& nativeName = nativeClassType->getNativeName();
	_dict->SetValue( "TYPE_NAME_CPP", nativeName );

	std::string headerName = nativeClassType->getHeaderName();
	headerName.erase( headerName.begin() );		// removes the '<'
	headerName.erase( headerName.end() - 1 );	// removes the '>'

	includeHeader( headerName );
	
	if( _mode == Mode_Code )
	{
		// add the "MY_LOCAL" section just for compatibility with interfaces
		ctemplate::TemplateDictionary* localDict = _dict->AddSectionDictionary( "MY_LOCAL" );
		fillAttribAccessors( nativeClassType, localDict );
		fillMethods( nativeClassType, localDict );
	}
}

// Struct to keep track of multiple interface inheritance ambiguities.
struct AmbiguousBase
{
	co::InterfaceType* base;	// the 'base' interface which causes ambiguity
	co::InterfaceType* super;	// the super-interface that should be used to disambiguate the base

	AmbiguousBase( co::InterfaceType* base, co::InterfaceType* super )
		: base( base ), super( super ) {;}

	bool operator<( const AmbiguousBase& other ) const
	{
		return base < other.base || ( base == other.base && super < other.super );
	}
};

typedef std::vector<AmbiguousBase> AmbiguousBases;

void gatherInheritedInterfaces( co::InterfaceType* itf, co::InterfaceType* originalSuper, AmbiguousBases& res )
{
	res.push_back( AmbiguousBase( itf, originalSuper ) );

	co::ArrayRange<co::InterfaceType* const> supers = itf->getSuperInterfaces();
	for( ; supers; supers.popFirst() )
	{
		co::InterfaceType* super = supers.getFirst();
		gatherInheritedInterfaces( super, originalSuper, res );
	}
}

void computeAmbiguousBases( co::InterfaceType* itf, AmbiguousBases& res )
{
	// gather all (directly and indirectly) inherited interfaces into 'res'
	co::ArrayRange<co::InterfaceType* const> supers = itf->getSuperInterfaces();
	for( ; supers; supers.popFirst() )
	{
		co::InterfaceType* super = supers.getFirst();
		gatherInheritedInterfaces( super, super, res );
	}

	/*
		Sort the vector (first by base, then by super) then iterate it backwards,
		removing all elements but the first sample of every duplicate 'base'.
	 */
	std::sort( res.begin(), res.end() );

	co::InterfaceType* lastBase = 0;
	for( std::size_t i = res.size() - 1; i > 0; --i )
	{
		AmbiguousBase& next = res[i - 1];
		AmbiguousBase& current = res[i];

		if( lastBase != current.base || current.base == next.base )
		{
			lastBase = current.base;
			res.erase( res.begin() + i );
			continue;
		}
	}

	if( res.front().base != lastBase )
		res.erase( res.begin() );
}

void TypeDictionaryBuilder::fillInterfaceData( co::InterfaceType* interfaceType )
{
	if( _mode == Mode_Mapping )
	{
		co::ArrayRange<co::InterfaceType* const> superTypes = interfaceType->getSuperInterfaces();
		assert( !superTypes.isEmpty() );

		// find and generate info for resolving interface ambiguities
		AmbiguousBases ambiguousBases;
		computeAmbiguousBases( interfaceType, ambiguousBases );

		if( !ambiguousBases.empty() )
		{
			_dict->ShowSection( "HAS_AMBIGUOUS_BASES" );
			_dict->SetValue( "FIRST_SUPERTYPE", getCppName( superTypes.getFirst() ) );

			std::size_t count = ambiguousBases.size();
			for( std::size_t i = 0; i < count; ++i )
			{
				AmbiguousBase& ab = ambiguousBases[i];
				ctemplate::TemplateDictionary* baseDict = _dict->AddSectionDictionary( "AMBIGUOUS_BASE_LIST" );
				baseDict->SetValue( "BASE", getCppName( ab.base ) );
				baseDict->SetValue( "SUPER", getCppName( ab.super ) );
			}
		}

		// fill in the interface super-types
		for( ; superTypes; superTypes.popFirst() )
		{
			co::InterfaceType* superType = superTypes.getFirst();
			includeType( superType );

			ctemplate::TemplateDictionary* supDict = _dict->AddSectionDictionary( "SUPERTYPE_LIST" );
			supDict->SetValue( "SUPERTYPE", getCppName( superType ) );
		}

		fillAttribAccessors( interfaceType, _dict );
		fillMethods( interfaceType, _dict );
	}
	else
	{
		if( interfaceType->getFullName() == "co.Reflector" )
		   _dict->ShowSection( "IS_REFLECTOR_REFLECTOR" );

		/*
			Reflectors must know about interface ancestors, their methods and attributes.
			For convenience, we also include the interface type as its own ancestor.
		 */
		co::ArrayRange<co::InterfaceType* const> ancestors = interfaceType->getInterfaceAncestors();
		for( ; ancestors; ancestors.popFirst() )
		{
			co::InterfaceType* ancestor = ancestors.getFirst();

			// exclude co.Interface
			if( ancestor->getFullName() == "co.Interface" )
				continue;

			fillInterfaceAncestor( ancestor );
		}

		if( interfaceType->getFullName() != "co.Interface" )
			fillInterfaceAncestor( interfaceType );

		// repeat the local members in a separate "MY_LOCAL" section
		ctemplate::TemplateDictionary* localDict = _dict->AddSectionDictionary( "MY_LOCAL" );
		fillAttribAccessors( interfaceType, localDict );
		fillMethods( interfaceType, localDict );
	}
}

void TypeDictionaryBuilder::fillInterfaceAncestor( co::InterfaceType* ancestor )
{
	ctemplate::TemplateDictionary* ancestorDict = _dict->AddSectionDictionary( "ANCESTOR_LIST" );
	ancestorDict->SetValue( "ANCESTOR_CLASS", getCppName( ancestor ) );
	fillAttribAccessors( ancestor, ancestorDict );
	fillMethods( ancestor, ancestorDict );
}

void TypeDictionaryBuilder::fillAttribAccessors( co::AttributeContainer* ac, ctemplate::TemplateDictionary* dict )
{
	co::ArrayRange<co::AttributeInfo* const> attributes = ac->getMemberAttributes();

	dict->ShowSection( attributes.isEmpty() ? "HAS_ATTRIBUTES_FALSE" : "HAS_ATTRIBUTES_TRUE" );

	for( ; attributes; attributes.popFirst() )
	{
		ctemplate::TemplateDictionary* attributeDict = dict->AddSectionDictionary( "ATTRIBUTE_LIST" );

		co::AttributeInfo* ai = attributes.getFirst();

		if( _mode == Mode_Code )
			attributeDict->SetIntValue( "ATTRIBUTE_INDEX", ai->getIndex() );

		co::Type* type = ai->getType();

		// getter
		ctemplate::TemplateDictionary* getterDict = attributeDict->AddSectionDictionary( "GETTER" );

		std::string getterName;
		co::LexicalUtilities::formatAccessor( ai->getName(), co::LexicalUtilities::Getter, getterName );
		getterDict->SetValue( "METHOD_NAME", getterName );

		std::string typeName;
		formatInputType( type, typeName );
		getterDict->SetValue( "RETURN_TYPE", typeName );

		includeType( type );

		if( ai->getIsReadOnly() )
		{
			if( _mode == Mode_Code )
				attributeDict->ShowSection( "IS_READ_ONLY" );

			continue;
		}

		// setter
		ctemplate::TemplateDictionary* setterDict = attributeDict->AddSectionDictionary( "SETTER" );

		std::string setterName;
		co::LexicalUtilities::formatAccessor( ai->getName(), co::LexicalUtilities::Setter, setterName );
		setterDict->SetValue( "METHOD_NAME", setterName );
		setterDict->SetValue( "PARAMETER_TYPE", typeName );
		setterDict->SetValue( "PARAMETER_NAME", ai->getName() );
	}
}

void TypeDictionaryBuilder::fillMethods( co::MethodContainer* mc, ctemplate::TemplateDictionary* dict )
{
	co::ArrayRange<co::MethodInfo* const> methods = mc->getMemberMethods();

	dict->ShowSection( methods.isEmpty() ? "HAS_METHODS_FALSE" : "HAS_METHODS_TRUE" );

	for( ; methods; methods.popFirst() )
	{
		ctemplate::TemplateDictionary* methodDict = dict->AddSectionDictionary( "METHOD_LIST" );

		co::MethodInfo* mi = methods.getFirst();

		if( _mode == Mode_Code )
			methodDict->SetIntValue( "METHOD_INDEX", mi->getIndex() );

		methodDict->SetValue( "METHOD_NAME", mi->getName() );

		co::Type* returnType = mi->getReturnType();
		if( returnType )
		{
			includeType( returnType );

			if( _mode == Mode_Code )
				methodDict->ShowSection( "HAS_RETURN" );
		}

		std::string returnTypeName;
		formatInputType( returnType, returnTypeName );
		methodDict->SetValue( "RETURN_TYPE", returnTypeName );

		co::ArrayRange<co::ParameterInfo* const> parameters = mi->getParameters();

		if( _mode == Mode_Code )
			methodDict->SetIntValue( "PARAMETER_COUNT", static_cast<int>( parameters.getSize() ) );

		methodDict->ShowSection( parameters.isEmpty() ? "HAS_PARAMETERS_FALSE" : "HAS_PARAMETERS_TRUE" );

		int parameterIndex = 0;
		for( ; parameters; parameters.popFirst() )
		{
			co::ParameterInfo* pi = parameters.getFirst();

			includeType( pi->getType() );

			std::string paramTypeName;
			if( pi->getIsOut() )
				formatOutputType( pi->getType(), paramTypeName );
			else
				formatInputType( pi->getType(), paramTypeName );

			ctemplate::TemplateDictionary* paramDict = methodDict->AddSectionDictionary( "PARAMETER_LIST" );
			paramDict->SetValue( "PARAMETER_TYPE", paramTypeName );
			paramDict->SetValue( "PARAMETER_NAME", pi->getName() );

			if( _mode == Mode_Code )
				paramDict->SetIntValue( "PARAMETER_INDEX", parameterIndex++ );
		}
	}
}

void TypeDictionaryBuilder::fillComponentData( co::ComponentType* componentType )
{
	if( _mode == Mode_Mapping )
		return;

	co::ArrayRange<co::InterfaceInfo* const> clientInterfaces = componentType->getClientInterfaces();
	_dict->ShowSection( clientInterfaces.isEmpty() ? "HAS_CLIENT_INTERFACES_FALSE" : "HAS_CLIENT_INTERFACES_TRUE" );
	fillInterfaceInfos( clientInterfaces, "CLIENT_INTERFACE_LIST" );

	co::ArrayRange<co::InterfaceInfo* const> serverInterfaces = componentType->getServerInterfaces();
	_dict->ShowSection( serverInterfaces.isEmpty() ? "HAS_SERVER_INTERFACES_FALSE" : "HAS_SERVER_INTERFACES_TRUE" );
	fillInterfaceInfos( serverInterfaces, "SERVER_INTERFACE_LIST" );
}

void TypeDictionaryBuilder::fillInterfaceInfos( co::ArrayRange<co::InterfaceInfo* const> interfaces,
													const char* sectionName )
{
	for( ; interfaces; interfaces.popFirst() )
	{
		co::InterfaceInfo* itf = interfaces.getFirst();

		co::InterfaceType* type = itf->getType();
		includeType( type );

		ctemplate::TemplateDictionary* interfaceDict = _dict->AddSectionDictionary( sectionName );

		interfaceDict->SetIntValue( "INDEX", itf->getIndex() );
		
		std::string name( itf->getName() );
		interfaceDict->SetValue( "NAME", name );

		name[0] = toupper( name[0] );
		interfaceDict->SetValue( "NAME_CAPITALIZED", name );

		interfaceDict->SetValue( "ITF_TYPE_FULL", type->getFullName() );
		interfaceDict->SetValue( "ITF_TYPE_CPP", getCppName( type ) );

		std::string fullNameWithUnderscores( type->getFullName() );
		convertSeparators( fullNameWithUnderscores, '.', '_' );

		interfaceDict->SetValue( "ITF_TYPE_FULL_UNDERSCORES", fullNameWithUnderscores );
	}
}

std::string TypeDictionaryBuilder::getKindName( co::Type* type )
{
	assert( type->getKind() >= co::TK_ENUM );

	static const char* kindNames[] =
	{
		"TK_ENUM",
		"TK_EXCEPTION",
		"TK_STRUCT",
		"TK_NATIVECLASS",
		"TK_INTERFACE",
		"TK_COMPONENT"
	};

	return kindNames[type->getKind() - co::TK_ENUM];
}

void TypeDictionaryBuilder::formatInputType( co::Type* type, std::string& out )
{
	if( !type )
	{
		out = "void";
		return;
	}

	co::TypeKind kind =  type->getKind();
	if( kind == co::TK_ARRAY )
	{
		includeArrayRangeHeader();
		co::Type* elementType = dynamic_cast<co::ArrayType*>( type )->getElementType();
		if( elementType->getKind() == co::TK_INTERFACE )
			concatenate( "co::ArrayRange<", getCppName( elementType ), "* const>", out );
		else
			concatenate( "co::ArrayRange<", getCppName( elementType ), " const>", out );
	}
	else if( ( kind >= co::TK_BOOLEAN && kind <= co::TK_DOUBLE ) || kind == co::TK_ENUM )
	{
		out = getCppName( type );
	}
	else if( kind == co::TK_STRING || kind == co::TK_ANY || kind == co::TK_STRUCT || kind == co::TK_NATIVECLASS )
	{
		concatenate( "const ", getCppName( type ), "&", out );
	}
	else if( kind == co::TK_INTERFACE )
	{
		out = getCppName( type );
		out += "*";
	}
	else
	{
		assert( false );
	}
}

void TypeDictionaryBuilder::formatOutputType( co::Type* type, std::string& out )
{
	co::TypeKind kind =  type->getKind();
	if( kind == co::TK_ARRAY )
	{
		co::Type* baseType = dynamic_cast<co::ArrayType*>( type )->getElementType();
		if( baseType->getKind() == co::TK_INTERFACE )
		{
			includeRefVectorHeader();
			concatenate( "co::RefVector<", getCppName( baseType ), ">*", out );
		}
		else
		{
			includeStdVectorHeader();
			concatenate( "std::vector<", getCppName( baseType ), ">*", out );
		}
	}
	else if( ( kind >= co::TK_BOOLEAN && kind <= co::TK_DOUBLE ) || kind == co::TK_ENUM )
	{
		out = getCppName( type );
		out += "&";
	}

	else if( kind == co::TK_STRING || kind == co::TK_ANY || kind == co::TK_STRUCT || kind == co::TK_NATIVECLASS )
	{
		out = getCppName( type );
		out += "&";
	}
	else if( kind == co::TK_INTERFACE )
	{
		out = getCppName( type );
		out += "*&";
	}
	else
		assert( false );
}

void TypeDictionaryBuilder::includeType( co::Type* type )
{
	co::TypeKind kind = type->getKind();
	if( kind >= co::TK_BOOLEAN && kind < co::TK_ARRAY )
		return;

	switch( kind )
	{
	case co::TK_ANY:
		includeHeader( "co/Any.h" );
		break;

	case co::TK_STRING:
		break;

	case co::TK_ARRAY:
		{
			co::Type* elementType = dynamic_cast<co::ArrayType*>( type )->getElementType();
			if( _type->getKind() == co::TK_STRUCT )
				includeHeader( elementType );
			else
				includeType( elementType );
		}
		break;

	case co::TK_ENUM:
	case co::TK_EXCEPTION:
		includeHeader( type );
		break;

	case co::TK_STRUCT:
		if( _type->getKind() == co::TK_STRUCT )
			includeHeader( type );
		else
			addForwardDeclaration( type );
		break;

	case co::TK_NATIVECLASS:
		addForwardDeclaration( type );
		break;

	case co::TK_INTERFACE:
		if( _type->getKind() == co::TK_INTERFACE && dynamic_cast<co::InterfaceType*>( _type )->
				isSubTypeOf( dynamic_cast<co::InterfaceType*>( type ) ) )
			includeHeader( type ); // '_type' must inherit from 'type'
		else
			addForwardDeclaration( type );
		break;

	default:
		assert( false );
	}
}

void TypeDictionaryBuilder::includeHeader( const std::string& headerName )
{
	_includes.insert( headerName );
}

void TypeDictionaryBuilder::includeHeader( co::Type* type )
{
	std::string filename( type->getFullName() );
	typeToHeaderName( filename );
	includeHeader( filename );
}

void TypeDictionaryBuilder::addForwardDeclaration( co::Type* type )
{
	if( _type == type )
		return; // fw-decl not needed for our own type

	_forwardDeclarations.push_back( type );
}

void TypeDictionaryBuilder::formatFieldType( co::Type* type, std::string& out )
{
	if( type->getKind() == co::TK_ARRAY )
	{
		co::Type* elementType = dynamic_cast<co::ArrayType*>( type )->getElementType();
		if( elementType->getKind() == co::TK_INTERFACE )
		{
			includeRefVectorHeader();
			concatenate( "co::RefVector<", getCppName( elementType ), ">", out );
		}
		else
		{
			includeStdVectorHeader();
			concatenate( "std::vector<", getCppName( elementType ), ">", out );
		}
	}
	else if( type->getKind() == co::TK_INTERFACE )
	{
		includeRefPtrHeader();
		concatenate( "co::RefPtr<", getCppName( type ), ">", out );
	}
	else
	{
		out = getCppName( type );
	}
}
