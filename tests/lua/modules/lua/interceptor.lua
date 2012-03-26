-- Used in LuaTests.cpp's interceptor test

local M = {}

function M.basic()
	-- one getField() and one invoke()
	local arrayType = co.system.types:getType( "int32[]" )

	-- one setField()
	arrayType.reflector = nil
end

function M.extra()
	co.Type.uint64.reflector = co.Type.uint64.currentReflector
end

return M
