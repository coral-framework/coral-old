/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <core/csl/Error.h>
#include <gtest/gtest.h>

#define THE_EXPECTED_ERROR_LINE_DOES_NOT_MATCH 0
#define THE_EXPECTED_ERROR_FILE_DOES_NOT_MATCH 0
#define THE_EXPECTED_ERROR_COUNT_DOES_NOT_MATCH 0
#define THE_EXPECTED_ERROR_MESSAGE_DOES_NOT_MATCH 0

CSLTester::CSLTester( const std::string& typeName, const LocationInfo& li  )
	: _typeName( typeName ),  _loader( typeName, co::getPaths(), co::getSystem()->getTypes() ), _locationInfo( li )
{
	// empty
}

CSLTester& CSLTester::expectedError( const std::string& messageSubStr, const std::string& filename, int line )
{
	ExpectedError e;
	e.filename = filename;
	e.line = line;
	e.messageSubStr = messageSubStr;
	_expectedErrors.push_back( e );

	return *this;
}

void CSLTester::run()
{
	#define CSL_LOCATION "[CSLTester]: Error, from: " << _locationInfo.sourceFile << ":" << _locationInfo.sourceLine << ": "

	ASSERT_NO_THROW( _loader.loadType() )
		<< CSL_LOCATION  << "the CSL file for type '" << _typeName << "' does not exist!";

	const csl::Error* error = _loader.getError();

	if( _expectedErrors.size() == 0 )
	{		
		// no errors were expected
		ASSERT_TRUE( error == NULL ) << CSL_LOCATION << "no errors were expected at the loading of type '"
									 << _typeName << "', but the loader has returned errors." << std::endl
									 << "Actual error stack is: " << *error;
	}
	else
	{
		ASSERT_TRUE( error != NULL ) << CSL_LOCATION << _expectedErrors.size()
									 << " error(s) were expected at the loading of type '"
									 << _typeName << "', but the loader has returned no errors.";

		// check the error hierarchy
		unsigned int i = 0;
		while( error != NULL )
		{
			// assert error line
			if( error->getLine() != _expectedErrors[i].line )
				ASSERT_TRUE( THE_EXPECTED_ERROR_LINE_DOES_NOT_MATCH ) << CSL_LOCATION
					<< "expected error of number " << i << " does not match actual error, "
					<< "line number differs. Actual: " << error->getLine() << ". "
					<< "Expected: " << _expectedErrors[i].line;

			// assert error file
			if( !TestHelper::stringEndsWith( error->getFileName(), _expectedErrors[i].filename ) )
				ASSERT_TRUE( THE_EXPECTED_ERROR_FILE_DOES_NOT_MATCH ) << CSL_LOCATION
					<< "expected error of number " << i << " does not match the actual error, "
					<< "filename differs. Actual: '" << error->getFileName() << "'. "
					<< "Expected: '" << _expectedErrors[i].filename << "'";

			// assert match of the error message substring
			std::string::size_type found = error->getMessage().find( _expectedErrors[i].messageSubStr );
			if( found == std::string::npos )
				ASSERT_TRUE( THE_EXPECTED_ERROR_MESSAGE_DOES_NOT_MATCH ) << CSL_LOCATION
					<< "expected error of number " << i << " does not match actual error, "
					<< "message does not match. Actual: " << error->getMessage() << ". "
					<< "Expected substring: " << _expectedErrors[i].messageSubStr;

			error = error->getInnerError();
			++i;
		}

		if( i != _expectedErrors.size() )
			ASSERT_TRUE( THE_EXPECTED_ERROR_MESSAGE_DOES_NOT_MATCH ) << CSL_LOCATION
					<< "actual error count is "
					<< i << " but expected error count is "
					<< _expectedErrors.size() << ".";
	}
}
