require "lua.test"

local M = {}

function M:initialize( module )
	-- create a co.CSLError struct instance
	local cslError = co.new( "co.CSLError" )

	-- test getting some attributes
	ASSERT_EQ( cslError.message, "" )
	ASSERT_EQ( cslError.filename, "" )
	ASSERT_EQ( cslError.line, 0 )

	-- test getting a non-existing attribute
	ASSERT_ERROR( function() return cslError.dummy end, "non-existing member 'dummy'" )

	-- test setting some attributes
	cslError.message = "My Message"
	ASSERT_EQ( cslError.message, "My Message" )
	cslError.filename = "My Filename"
	ASSERT_EQ( cslError.filename, "My Filename" )
	cslError.line = 1337
	ASSERT_EQ( cslError.line, 1337 )

	-- test setting a non-existing attribute
	ASSERT_ERROR( function() cslError.dummy = 2 end, "non-existing member 'dummy'" )

	-- create a moduleA.TestStruct struct instance
	local testStruct = co.new( "moduleA.TestStruct" )

	-- test getting some attributes
	ASSERT_EQ( testStruct.testEnum, "First" )
	ASSERT_EQ( testStruct.aType, nil )

	-- test setting some attributes
	testStruct.testEnum = "Third"
	ASSERT_EQ( testStruct.testEnum, "Third" )
	testStruct.aType = co.getType( "uint16" )
	ASSERT_EQ( testStruct.aType.fullName, "uint16" )

	-- test setting 'testEnum' with an invalid identifier
	ASSERT_ERROR( function() testStruct.testEnum = "Fourth" end,
		"invalid identifier 'Fourth' for enum 'moduleA.TestEnum'" )

	-- test getting attributes of the innerStruct
	ASSERT_EQ( testStruct.innerStruct.message, "" )
	ASSERT_EQ( testStruct.innerStruct.filename, "" )
	ASSERT_EQ( testStruct.innerStruct.line, 0 )

	-- test setting attributes of the innerStruct
	testStruct.innerStruct.message = "Something"
	ASSERT_EQ( testStruct.innerStruct.message, "" ) -- FIX THIS

	-- test setting the innerStruct attribute itself
	testStruct.innerStruct = cslError
	ASSERT_EQ( testStruct.innerStruct.message, "My Message" )
	ASSERT_EQ( testStruct.innerStruct.filename, "My Filename" )
	ASSERT_EQ( testStruct.innerStruct.line, 1337 )

end

return M
