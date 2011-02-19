local M = {}

function M.passthrough( ... )
	return ...
end

function M.constants()
	return nil, true, false, 3.14, "hello", co.system
end

function M.constants2()
	return "one", "two", "three", "four", "five"
end

function M.raiseError()
	error "here is the error"
end

return M
