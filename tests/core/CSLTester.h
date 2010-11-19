/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CSLTESTER_H_
#define _CSLTESTER_H_

#include "TestHelper.h"
#include <core/TypeLoader.h>

//! Expands to a LocationInfo with info about the line file
#ifndef CLS_TESTER_LOCATION_INFO
#define CLS_TESTER_LOCATION_INFO CSLTester::LocationInfo( __FILE__, __LINE__ )
#endif

/*
	Utility class used to test CSL file loading.
	It supports a stack of expected errors that must occur during the loading of a type.
	If no expected errors are set, then the type is suposed to load successfully (without errors).
	The test is performed using gtest macros with custom error messages.
 */
class CSLTester
{
public:
	struct LocationInfo
	{
		LocationInfo()
			: sourceLine( -1 )
		{;}

		LocationInfo( const std::string file, int line )
			: sourceFile( file ), sourceLine( line )
		{;}

		std::string sourceFile;
		int sourceLine;
	};

public:
	CSLTester( const std::string& typeName, const LocationInfo& li );

	// Pushes the filename and the line at which the error must occur. In the case of a
	// expected semantic error, no filename nor line are available.
	// If an messageSubStr is non-empty, then the tester will search for a match of that string
	// within the error message
	CSLTester& expectedError( const std::string& messageSubStr, const std::string& filename, int line );

	// Runs the test: tries to load the specified type and checks all expected errors
	void run();

private:
	struct ExpectedError
	{
		std::string filename;
		int line;
		std::string messageSubStr;
	};

	typedef std::vector<ExpectedError> ExpectedErrorList;
	ExpectedErrorList _expectedErrors;

	std::string _typeName;

	TypeLoader _loader;
	LocationInfo _locationInfo;
};

#ifndef CSL_TEST_BEGIN

// test a CSL file loading, expecting errors that must be specified
#define CSL_TEST_BEGIN( typeName ) CSLTester( typeName, CLS_TESTER_LOCATION_INFO )
#define CSL_EXPECT_ERROR( message, file, line ) .expectedError( message, file, line )
#define CSL_EXPECT_SYNTAX_ERROR( file, line ) .expectedError( "syntax error", file, line )
#define CSL_EXPECT_SEMANTIC_ERROR( messageSubStr ) .expectedError( messageSubStr, "", -1 )
#define CSL_TEST_END() .run();

// test a CSL file loading, expecting no errors
#define CSL_TEST( typeName ) CSL_TEST_BEGIN( typeName ).run();
#endif

#endif
