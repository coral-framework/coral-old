require "lua.test"

local M = {}

function M:initialize( module )
	local systemType = co.system.types:findType( "co.System" )
	ASSERT_EQ( systemType.fullName, "co.System" )

	ASSERT_ERROR( function() co.system.services:getService( systemType ) end, "unknown service" )

	-- TODO more tests
end

return M
