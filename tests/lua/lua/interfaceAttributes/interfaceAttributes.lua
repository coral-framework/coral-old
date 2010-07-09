local M = {}

function M:initialize( module )
	-- test getting/setting a boolean
	assert( co.system.services.isLazy == true )
	co.system.services.isLazy = false
	assert( co.system.services.isLazy == false )
	co.system.services.isLazy = true
	assert( co.system.services.isLazy == true )

	-- test getting/setting an int8
	-- test getting/setting an uint8
	-- test getting/setting an int16
	-- test getting/setting an uint16
	-- test getting/setting an int32
	-- test getting/setting an uint32
	-- test getting/setting an int64
	-- test getting/setting an uint64
	-- test getting/setting a float
	-- test getting/setting a double
	-- test getting/setting a string
	-- test getting/setting an enum
	-- test getting/setting a struct
	-- test getting/setting a native class
	-- test getting/setting an interface
	-- test getting/setting a simple array
	-- test getting/setting a struct array
	-- test getting/setting an interface array
end

return M
