/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ModuleCompiler.h"
#include "StreamEmitter.h"
#include "HelperFunctions.h"
#include "MappingGenerator.h"
#include "TypeDictionaryBuilder.h"

#include <co/Type.h>
#include <co/Uuid.h>
#include <co/Coral.h>
#include <co/Config.h>
#include <co/System.h>
#include <co/Platform.h>
#include <co/Exception.h>
#include <co/Namespace.h>
#include <co/ArrayRange.h>
#include <co/ModulePart.h>
#include <co/TypeManager.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>
#include <co/ComponentType.h>

#include <core/tools/Dir.h>
#include <core/tools/StringTokenizer.h>

#include <ctemplate/template.h>
#include <glog/logging.h>
#include <sstream>

bool isStandardBuiltInType( const std::string& name )
{
	static std::set<std::string> s_typeNames;
	if( s_typeNames.empty() )
	{
		s_typeNames.insert( "co.Interface" );
		s_typeNames.insert( "co.TypeKind" );
	}
	return s_typeNames.find( name ) != s_typeNames.end();
}

ModuleCompiler::ModuleCompiler( const std::string& moduleName, const std::string& codeDir, const std::string& mappingsDir )
	: _moduleName( moduleName ), _codeDir( codeDir ), _mappingGenerator( mappingsDir )
{
	_isGeneratingModule = ( !_moduleName.empty() );

	_numGeneratedMappings = 0;
	_customModulePartDefined = false;

	_moduleDirPath = _moduleName;
	for( std::size_t i = 0; i < _moduleDirPath.length(); ++i )
		if( _moduleDirPath[i] == '.' )
			_moduleDirPath[i] = '/';
}

ModuleCompiler::~ModuleCompiler()
{
	// empty
}

std::size_t countTypesIn( co::Namespace* ns )
{
	std::size_t res = ns->getTypes().getSize();
	for( co::ArrayRange<co::Namespace* const> r( ns->getChildNamespaces() ); r; r.popFirst() )
		res += countTypesIn( r.getFirst() );
	return res;
}

int ModuleCompiler::run()
{
	try
	{
		initialize();

		std::size_t totalNumTypes = countTypesIn( co::getSystem()->getTypes()->getRootNS() );
		LOG( INFO ) << "Loaded " << totalNumTypes << " types.";

		// keep track of (and generate mappings for) all module dependencies
		co::DependencyWalker depWalker( _moduleDependencies.begin(), _moduleDependencies.end() );
		depWalker.walk( this ); // triggers onTypeVisited()

		LOG( INFO ) << "Created " << _numGeneratedMappings << " mappings.";

		if( _isGeneratingModule )
		{
			LOG( INFO ) << "Generating module code...";

			generateCodeForTypes();
			generateCodeForModule();

			LOG( INFO ) << "Done.";
		}

		return 0;
	}
	catch( std::exception& e )
	{
		LOG( ERROR ) << "*** Error ***\n" << e.what();
		return -1;
	}
	catch( ... )
	{
		LOG( FATAL ) << "Oops, caught an unknown exception!";
		return -2;
	}
}

void ModuleCompiler::initialize()
{
	assert( _moduleTypes.empty() );

	setupCTemplate();

	if( _isGeneratingModule )
		loadModuleTypes();

	// add the explicitly requested 'extra' module dependencies
	for( StringList::iterator it = _extraDependencies.begin(); it != _extraDependencies.end(); ++it )
	{
		_moduleDependencies.insert( co::getType( *it ) );
	}

	/*
		Add other implicit module dependencies. Sometimes they don't come as
		explicit dependencies, but generated code depends on them.
	 */
	_moduleDependencies.insert( co::getType( "co.System" ) );
	_moduleDependencies.insert( co::getType( "co.Namespace" ) );
	_moduleDependencies.insert( co::getType( "co.Reflector" ) );
	_moduleDependencies.insert( co::getType( "co.ModulePart" ) );

	// all type classes are implicit dependencies
	_moduleDependencies.insert( co::getType( "co.EnumType" ) );
	_moduleDependencies.insert( co::getType( "co.ArrayType" ) );
	_moduleDependencies.insert( co::getType( "co.StructType" ) );
	_moduleDependencies.insert( co::getType( "co.ExceptionType" ) );
	_moduleDependencies.insert( co::getType( "co.InterfaceType" ) );
	_moduleDependencies.insert( co::getType( "co.ComponentType" ) );
	_moduleDependencies.insert( co::getType( "co.NativeClassType" ) );
}

void ModuleCompiler::setupCTemplate()
{
	ctemplate::Template::SetTemplateRootDirectory( COMPILER_TEMPLATES_DIR );

	// set global CURRENT_DATE_TIME
	time_t itime = time( NULL );
	struct tm timeInfo;

#ifdef CORAL_OS_WIN
	localtime_s( &timeInfo, &itime );
#else
	localtime_r( &itime, &timeInfo );
#endif

	char buffer[64];
	strftime( buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", &timeInfo );

	ctemplate::TemplateDictionary::SetGlobalValue( "CURRENT_DATE_TIME", buffer );
	ctemplate::TemplateDictionary::SetGlobalValue( "CORAL_VERSION", CORAL_VERSION_STR );

	sprintf( buffer, "%i", CORAL_COMPILER_OUTPUT_REVISION );
	ctemplate::TemplateDictionary::SetGlobalValue( "OUTPUT_REVISION", buffer );
}

void ModuleCompiler::loadModuleTypes()
{
	// fully search the Coral path looking for module types (i.e. CSL files)
	co::ArrayRange<const std::string> pathsRange = co::getPaths();
	std::string dirPath, typeName;
	for( ; pathsRange; pathsRange.popFirst() )
	{
		const std::string& path = pathsRange.getFirst();
		
		dirPath.reserve( path.length() + _moduleDirPath.length() + 1 );
		dirPath = path;
		dirPath += "/";
		dirPath += _moduleDirPath;
		
		co::Dir dir( dirPath );
		if( !dir.exists() )
			continue;
		
		std::vector<std::string> files;
		dir.getEntryList( files );
		
		std::size_t fileCount = files.size();
		for( std::size_t i = 0; i < fileCount; ++i )
		{
			const std::string& filename = files[i];
			
			// does it have a '.csl' extension?
			std::size_t lastDotIndex = filename.rfind( '.' );
			if( lastDotIndex == std::string::npos || filename.length() - lastDotIndex != 4 )
				continue;
			
			const char* ext = filename.c_str() + lastDotIndex;
			if( tolower( ext[1] ) != 'c' || tolower( ext[2] ) != 's' || tolower( ext[3] ) != 'l' )
				continue;
			
			std::string typeName;
			typeName.reserve( _moduleName.length() + filename.length() + 1 );
			typeName += _moduleName;
			typeName += ".";
			typeName += filename;
			
			// remove the '.csl' extension
			typeName.resize( typeName.length() - 4 );
			
			co::Type* type = co::getType( typeName );
			_moduleTypes.push_back( type );
			
			// add the type's type interface as an implicit module dependency
			_moduleDependencies.insert( co::getType( std::string( "co." ) + getTypeInterfaceName( type ) ) );
		}
	}

	LOG( INFO ) << "Found " << _moduleTypes.size() << " module types.";

	// add all module types as module dependencies
	_moduleDependencies.insert( _moduleTypes.begin(), _moduleTypes.end() );
}

void ModuleCompiler::onTypeVisited( co::Type* type, co::uint32 depth )
{
	// track direct module dependencies (also includes the module's own types)
	if( depth <= 1 )
		_moduleDependencies.insert( type );

	// special cases: nothing is generated for these standard built-in types
	const std::string& name = type->getFullName();
	if( isStandardBuiltInType( name ) )
		return;

	TypeDictionaryBuilder typeDictBuilder( type );
	typeDictBuilder.buildMappingDict();

	// generate a mapping, unless the type is a component
	if( type->getKind() != co::TK_COMPONENT )
	{
		++_numGeneratedMappings;
		_mappingGenerator.generate( type, typeDictBuilder.getDictionary() );
	}

	// if a component with the module's name is defined, check its integrity
	if( type->getKind() == co::TK_COMPONENT && type->getName() == _moduleName )
	{
		co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
		co::InterfaceType* modulePartInterface = co::typeOf<co::ModulePart>::get();

		co::ArrayRange<co::InterfaceInfo* const> range = componentType->getProvidedInterfaces();
		for( ; range; range.popFirst() )
			if( range.getFirst()->getType()->isSubTypeOf( modulePartInterface ) )
				break;

		if( range.isEmpty() )
			CORAL_THROW( co::Exception, "Module '" << _moduleName << "' contains a component named '"
						<< _moduleName << "', but it does not provide the co.ModulePart interface." );

		if( range.getFirst()->getName() != "part" )
			CORAL_THROW( co::Exception, "Module '" << _moduleName << "' contains a component named '"
						<< _moduleName << "', but its co.ModulePart interface is not named 'part'." );

		_customModulePartDefined = true;
	}
}

void ModuleCompiler::generateCodeForTypes()
{
	for( TypeList::iterator it = _moduleTypes.begin(); it != _moduleTypes.end(); ++it )
	{
		co::Type* type = *it;

		TypeDictionaryBuilder typeDictBuilder( type );
		typeDictBuilder.buildCodeDict();
		
		switch( type->getKind() )
		{
		case co::TK_NATIVECLASS:
			{
				FileStreamEmitter fileStream( _codeDir, type->getName() + "_Adapter.h" );
				fileStream.expand( "nativeclass_adapter.h.tpl", typeDictBuilder.getDictionary() );
			}
			break;

		case co::TK_COMPONENT:
			{
				FileStreamEmitter fileStream( _codeDir, type->getName() + "_Base.h" );
				fileStream.expand( "component_base.h.tpl", typeDictBuilder.getDictionary() );
				
				fileStream.reopen( _codeDir, type->getName() + "_Base.cpp" );
				fileStream.expand( "component_base.cpp.tpl", typeDictBuilder.getDictionary() );
			}
			break;

		default:
			break;
		}

		generateReflector( type, typeDictBuilder.getDictionary() );
	}
}

void ModuleCompiler::generateReflector( co::Type* type, ctemplate::TemplateDictionary* dictionary )
{
	co::TypeKind kind = type->getKind();

	// set all "toggle" sections according to type kind
	dictionary->ShowSection( kind == co::TK_NATIVECLASS ? "IS_NATIVE_TRUE" : "IS_NATIVE_FALSE" );
	dictionary->ShowSection( kind == co::TK_INTERFACE ? "IS_INTERFACE_TRUE" : "IS_INTERFACE_FALSE" );
	dictionary->ShowSection( kind == co::TK_COMPONENT ? "IS_COMPONENT_TRUE" : "IS_COMPONENT_FALSE" );
	
	if( kind == co::TK_NATIVECLASS )
	{
		dictionary->SetFormattedValue( "CALL_PREFIX", "%s::%s_Adapter::",
			getCppName( type->getNamespace()->getFullName() ).c_str(), type->getName().c_str() );
	}
	else
	{
		dictionary->SetValue( "CALL_PREFIX", "p->" );
	}

	if( kind == co::TK_STRUCT || kind == co::TK_NATIVECLASS )
		dictionary->ShowSection( "IS_COMPLEX_VALUE" );

	if( dynamic_cast<co::CompoundType*>( type ) != NULL )
		dictionary->ShowSection( "IS_COMPOUND_TYPE" );

	if( dynamic_cast<co::MethodContainer*>( type ) != NULL )
		dictionary->ShowSection( "IS_METHOD_CONTAINER" );

	FileStreamEmitter fileStream( _codeDir, type->getName() + "_Reflector.cpp" );
	fileStream.expand( "reflector_header.tpl", dictionary );

	switch( kind )
	{
	case co::TK_STRUCT:
		fileStream.expand( "reflector_struct.tpl", dictionary );
		break;

	case co::TK_NATIVECLASS:
	case co::TK_INTERFACE:
		fileStream.expand( "reflector_methodcontainers.tpl", dictionary );
		break;

	default:
		break;
	}

	fileStream.expand( "reflector_footer.tpl", dictionary );
}

void ModuleCompiler::generateCodeForModule()
{
	// setup the module's template dictionary
	ctemplate::TemplateDictionary* moduleDict = new ctemplate::TemplateDictionary( "module" );

	ctemplate::TemplateDictionary* revCheckDict = moduleDict->AddIncludeDictionary( "REVISION_CHECK" );
	revCheckDict->SetFilename( "revision_check.tpl" );

	moduleDict->SetValue( "MODULE_NAME", _moduleName );

	std::string moduleNameUnderscores( _moduleName );
	convertSeparators( moduleNameUnderscores, '.', '_' );
	moduleDict->SetValue( "MODULE_NAME_UNDERSCORES", moduleNameUnderscores );

	// add the NAMESPACE_LIST
	co::StringTokenizer st( _moduleName, "." );
	while( st.nextToken() )
	{
		ctemplate::TemplateDictionary* nsList = moduleDict->AddSectionDictionary( "NAMESPACE_LIST" );
		nsList->SetValue( "NAMESPACE", st.getToken() );
	}

	moduleDict->SetValue( "NS", getCppName( _moduleName ) );

	// fill data about the module types
	for( TypeList::iterator it = _moduleTypes.begin(); it != _moduleTypes.end(); ++it )
	{
		co::Type* type = *it;

		ctemplate::TemplateDictionary* typeDict = moduleDict->AddSectionDictionary( "MODULE_TYPE_LIST" );
		typeDict->SetValue( "TYPE_NAME", type->getName() );
	}

	std::string binarySignatureString;

	// fill data about the module dependencies
	for( TypeSet::iterator it = _moduleDependencies.begin(); it != _moduleDependencies.end(); ++it )
	{
		co::Type* type = *it;

		ctemplate::TemplateDictionary* depDict = moduleDict->AddSectionDictionary( "MODULE_DEPENDENCIES_LIST" );
		depDict->SetValue( "TYPE_NAME_FULL", type->getFullName() );

		type->getBinarySignature().getString( binarySignatureString );
		depDict->SetValue( "TYPE_BINARY_SIGNATURE", binarySignatureString );
	}

	FileStreamEmitter fileStream;

	fileStream.reopen( _codeDir, "__Bootstrap.cpp" );
	fileStream.expand( "module_bootstrap.tpl", moduleDict );

	fileStream.reopen( _codeDir, "ModuleInstaller.h" );
	fileStream.expand( "module_installer.h.tpl", moduleDict );

	fileStream.reopen( _codeDir, "ModuleInstaller.cpp" );
	fileStream.expand( "module_installer.cpp.tpl", moduleDict );

	if( !_customModulePartDefined )
	{
		fileStream.reopen( _codeDir, "__ModulePart.cpp" );
		fileStream.expand( "module_default_part.tpl", moduleDict );
	}

	delete moduleDict;
}
