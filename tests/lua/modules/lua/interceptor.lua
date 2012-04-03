-- Used in LuaTests.cpp's interceptor test

local M = {}

function M.get( obj, index )
	return obj[index]
end

function M.basicCalls()
	-- one getField() and one invoke()
	local arrayType = co.system.types:getType( "int32[]" )

	-- one setField()
	arrayType.reflector = nil
end

function M.extraCalls()
	co.Type.uint64.reflector = co.Type.uint64.currentReflector
end

return M
