/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _MAPPINGGENERATOR_H_
#define _MAPPINGGENERATOR_H_

#include <string>

namespace co {
	class Type;
}

namespace ctemplate {
	class TemplateDictionary;
}

/*!
	Class that generates a file mapping content based on the \c typeDictionary and the \c typeKind
 */
class MappingGenerator
{
public:
	MappingGenerator( const std::string& mappingsDir );

	//! Outputs the generated mapping to the specified stream.
	void generate( co::Type* type, ctemplate::TemplateDictionary* typeDictionary );

private:
	void computeDir( co::Type* type, std::string& dir );
	
private:
	std::string _mappingsDir;
};

#endif // _MAPPINGGENERATOR_H_
