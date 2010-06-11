/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "StringBuffer_Adapter.h"
#include <moduleA/TestException.h>

void moduleA::StringBuffer_Adapter::addString( std::stringstream& instance, const std::string& str )
{
	instance << str;
}

void moduleA::StringBuffer_Adapter::addNumber( std::stringstream& instance, double n )
{
	instance << n;
}

void moduleA::StringBuffer_Adapter::getResult( std::stringstream& instance, std::string& result )
{
	result = instance.str();
	if( result.empty() )
		throw moduleA::TestException( "the StringBuffer is empty" );
}

void moduleA::StringBuffer_Adapter::clear( std::stringstream& instance )
{
	instance.str( "" );
}
