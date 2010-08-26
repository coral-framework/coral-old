require "lua.test"

local M = {}

function M:initialize( module )
	-- create a test component
	local testComponent = co.new( "moduleA.TestComponent" )

	-- test getting the 'componentType' attribute
	ASSERT_EQ( testComponent.componentType.fullName, "moduleA.TestComponent" )

	-- test setting the 'componentType' attribute
	ASSERT_ERROR( function() testComponent.componentType = nil end,
		"attribute 'componentType' is read-only and cannot be changed" )

	-- test getting a non-existing component interface
	ASSERT_ERROR( function() return testComponent.nonExistingItf end, "non-existing member 'nonExistingItf'" )

	-- test getting existing interfaces
	ASSERT_EQ( testComponent.testInterface.interfaceName, "testInterface" )
	ASSERT_EQ( testComponent.testInterface.interfaceOwner, testComponent )

	ASSERT_EQ( testComponent.transaction.interfaceName, "transaction" )
	ASSERT_EQ( testComponent.transaction.interfaceOwner, testComponent )

	-- test getting/setting a client interface
	ASSERT_EQ( testComponent.itfType, nil )
	testComponent.itfType = testComponent.testInterface.interfaceType
	ASSERT_EQ( testComponent.itfType, testComponent.testInterface.interfaceType )
	testComponent.itfType = nil
	ASSERT_EQ( testComponent.itfType, nil )

	-- test setting a server interface
	ASSERT_ERROR( function() testComponent.testInterface = nil end,
		"'testInterface' is a provided interface and cannot be set" )
end

return M
