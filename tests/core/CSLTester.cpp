/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <core/csl/Error.h>
#include <gtest/gtest.h>

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
	#define CSL_LOCATION "[CSLTester] failed at " << _locationInfo.sourceFile << ":" << _locationInfo.sourceLine << ": "

	ASSERT_NO_THROW( _loader.loadType() )
		<< CSL_LOCATION  << "the CSL file for type '" << _typeName << "' does not exist!";

	const co::csl::Error* error = _loader.getError();

	if( _expectedErrors.size() == 0 )
	{		
		// no errors were expected
		ASSERT_TRUE( error == NULL )
			<< CSL_LOCATION << "no errors were expected at the loading of type '"
			<< _typeName << "', but the loader has returned errors." << std::endl
			<< "Actual error stack is: " << *error;
	}
	else
	{
		ASSERT_TRUE( error != NULL )
			<< CSL_LOCATION << _expectedErrors.size()
			<< " error(s) were expected at the loading of type '"
			<< _typeName << "', but the loader has returned no errors.";

		// check the error hierarchy
		unsigned int i = 0;
		while( error != NULL )
		{
			// assert error line
			if( error->getLine() != _expectedErrors[i].line )
				FAIL()
					<< CSL_LOCATION << "line number mismatch"
					<< " at level " << i << ". "
					<< "Expected: " << _expectedErrors[i].line
					<< ". Actual: " << error->getLine();

			// assert error file
			if( !TestHelper::stringEndsWith( error->getFileName(), _expectedErrors[i].filename ) )
				FAIL()
					<< CSL_LOCATION << "filename mismatch"
					<< " at level " << i << ". "
					<< "Expected: '" << _expectedErrors[i].filename
					<< "'. Actual: '" << error->getFileName();

			// assert match of the error message substring
			std::string::size_type found = error->getMessage().find( _expectedErrors[i].messageSubStr );
			if( found == std::string::npos )
				FAIL()
					<< CSL_LOCATION << "message mismatch"
					<< " at level " << i << ". "
					<< "Expected substring: '" << _expectedErrors[i].messageSubStr
					<< "'. Actual message: " << error->getMessage();

			error = error->getInnerError();
			++i;
		}

		if( i != _expectedErrors.size() )
			FAIL()
				<< CSL_LOCATION << "error count mismatch ("
				<< _expectedErrors.size() << " expected, got " << i << ")";
	}
}
