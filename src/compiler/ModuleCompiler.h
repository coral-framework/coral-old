/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _MODULECOMPILER_H_
#define _MODULECOMPILER_H_

#include "MappingGenerator.h"
#include <core/TypeLoader.h>
#include <core/tools/DependencyWalker.h>

namespace ctemplate {
	class TemplateDictionary;
}

/*!
	Manages the "compilation" of a module - i.e. all the source-code generation steps
	needed to successfully compile the dynamic library of a Coral module.
 */
class ModuleCompiler : private co::IDependencyVisitor
{
public:
	/*!
		Constructs a compiler for the module identified by \c moduleName (a full, dot-separated namespace name).
		Mappings are generated in the \c mappingsDir, while reflection/implementation code for
		the module types are generated in the \c implDir.
	 */
	ModuleCompiler( const std::string& moduleName, const std::string& codeDir, const std::string& mappingsDir );

	//! Destructor.
	virtual ~ModuleCompiler();

	//! Adds an extra type dependency for which the compiler should generate a mapping.
	inline void addExtraDependency( const std::string& typeName )
	{
		_extraDependencies.push_back( typeName );
	}

	/*!
		Loads all module types, then:
			1) Generates mappings for the whole graph of type dependencies;
			2) Generates reflection/implementation code for the module types;
		Returns zero if everything ran smoothly, or non-zero if an error ocurred.
	 */
	int run();

private:
	//! General initialization steps.
	void initialize();

	//! Sets global ctemplate options, as well as values used in all files.
	void setupCTemplate();

	/*!
		Loads all module types by locating CSL files in the module's namespace
			(which may be scattered in several dirs in the path).
	 */
	void loadModuleTypes();

	/*!
		ITypeVisitor method - called for all module types and their dependencies.
		This also gathers direct module dependencies into _moduleDependencies.
	 */
	virtual void onTypeVisited( co::Type* type, co::uint32 depth );

	//! Generates code for the module types.
	void generateCodeForTypes();

	//! Generates the reflector of a module type.
	void generateReflector( co::Type* type, ctemplate::TemplateDictionary* dictionary );

	//! Generates all per-module implementation files.
	void generateCodeForModule();

private:
	std::string _moduleName;
	std::string _moduleDirPath;
	std::string _codeDir;

	// true if we're compiling a module; false if we're only generating mappings
	bool _isGeneratingModule;

	typedef std::vector<std::string> StringList;
	StringList _extraDependencies;

	bool _customModulePartDefined;
	co::uint32 _numGeneratedMappings;

	typedef std::vector<co::Type*> TypeList;
	TypeList _moduleTypes;

	typedef std::set<co::Type*> TypeSet;
	TypeSet _moduleDependencies;

	MappingGenerator _mappingGenerator;
};

#endif // _CORALCOMPILER_H_
