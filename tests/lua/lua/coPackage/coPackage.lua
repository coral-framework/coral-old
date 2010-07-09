local M = {}

function M:initialize( module )
	-- test the co.system interface
	assert( co.system )
	assert( co.system.interfaceType.fullName == "co.System" )

	-- test the co.newInstance() function
	local componentA = co.newInstance( "moduleA.TestComponent" )
	assert( componentA.componentType.fullName == "moduleA.TestComponent" )
end

return M
