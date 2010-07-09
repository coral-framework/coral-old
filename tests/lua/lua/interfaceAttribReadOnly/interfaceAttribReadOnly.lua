local M = {}

function M:initialize( module )
	-- try setting a read-only attribute
	assert( co.system.state == 5 )
	co.system.state = 2
end

return M
