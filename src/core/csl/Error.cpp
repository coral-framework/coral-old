/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Error.h"
#include <ostream>

namespace co {
namespace csl {

Error::Error( const std::string& msg, const std::string& filename, int32 line, Error* innerError )
	: _message( msg ), _filename( filename ), _line( line ), _innerError( innerError )
{
	// empty
}

Error::~Error()
{
	// empty
}

} // namespace csl
} // namespace co

std::ostream& operator<<( std::ostream& out, const co::csl::Error& error )
{
	int level = 0;

	const co::csl::Error* currentError = &error;
	while( currentError )
	{
		for( int i = 0; i < level; ++i )
			out << '\t';

		if( currentError->getLine() == -1 )
		{
			out << "Error: " << currentError->getMessage() << ".\n";
			assert( !currentError->getInnerError() );
			return out;
		}

		const co::csl::Error* nextError = currentError->getInnerError();
		bool nextIsNotLast = ( nextError != NULL && nextError->getLine() != -1 );

		out << ( nextIsNotLast ? "From " : "In file " )
			<< currentError->getFileName() << ":" << currentError->getLine()
			<< ": " << currentError->getMessage() << ( nextIsNotLast ? ":\n" : ".\n" );

		++level;

		currentError = nextError;
	}

	return out;
}
