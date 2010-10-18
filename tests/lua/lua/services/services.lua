require "lua.test"

local M = {}

function M:initialize( module )
	-- make sure we don't have services registered for 'moduleA.IBat' nor 'moduleA.IHuman'.
	ASSERT_ERROR( function() co.getService "moduleA.IBat" end, "unknown service" )
	ASSERT_ERROR( function() co.getService "moduleA.IHuman" end, "unknown service" )

	-- we reuse "lua.bat.Component" from the lua.component test
	local bc = co.new "lua.bat.Component"

	-- register a global moduleA.IBat service:
	co.system.services:addService( co.Type "moduleA.IBat", bc.fruitBat )

	-- now all queries should return the global service
	ASSERT_EQ( co.getService "moduleA.IBat", bc.fruitBat )
	ASSERT_EQ( co.getService( "moduleA.IBat", "moduleA.IHuman" ), bc.fruitBat )

	-- now add a specialized bat for humans
	co.system.services:addServiceForType( co.Type "moduleA.IBat", co.Type "moduleA.IHuman", bc.vampireBat )
	ASSERT_EQ( co.getService "moduleA.IBat", bc.fruitBat )
	ASSERT_EQ( co.getService( "moduleA.IBat", "moduleA.IHuman" ), bc.vampireBat )

	-- a query for 'moduleA.IHuman' on 'bc' should give us batman
	ASSERT_EQ( co.getService( "moduleA.IHuman", bc ), bc.batman )
	ASSERT_EQ( co.getService( "moduleA.IHuman", bc.batman ), bc.batman )
	ASSERT_EQ( co.getService( "moduleA.IHuman", bc.fruitBat ), bc.batman )
end

return M
