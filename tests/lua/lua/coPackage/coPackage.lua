require "lua.test"

local M = {}

function M:initialize( module )
	-- test the co.system interface
	ASSERT_TRUE( co.system )
	ASSERT_EQ( co.system.interfaceType.fullName, "co.System" )

	-- test the co.newInstance() function
	local testComponent = co.newInstance( "moduleA.TestComponent" )
	ASSERT_EQ( testComponent.componentType.fullName, "moduleA.TestComponent" )
	ASSERT_EQ( testComponent.testInterface.interfaceName, "testInterface" )
	ASSERT_EQ( testComponent.testInterface.interfaceOwner, testComponent )
end

return M
