-------------------------------------------------------------------------------
-- Module for processing Command-Line Arguments.
-------------------------------------------------------------------------------
--
-- [leftOvers][,errorString] = process( args, handlers )
--
-- Processes flags in the 'args' array by calling functions in the 'handlers'
-- table, indexed by flag name. Returns an array of leftovers (unprocessed,
-- non-flag arguments), or nil and an error string on command-line error.
--
-- Handler functions receive the name of the flag that triggered their call,
-- plus any non-flag args following them (potential parameters for the flag).
-- They return the number of additional parameters used (nil = 0, 1..).
-- In case of error, the function should return nil and an error message.
--
-- Signature of a handler function:
--     [numUsedArgs][,errorString] = function( flagName [, ...] )
--
-- Sample 'handlers' table:
--     local flagHandlers = {
--         v = function() print( "Foo v0.1" ) end,
--         version = 'v',
--     }
--
-------------------------------------------------------------------------------
-- Inspired by a submission to lua-l by Asko Kauppi.
-- Ideas for future enhancements:
--    * Flag names as Lua patterns, instead of plain strings (Xavante style).
--		This would mean support for captures, which could be passed to the
--		handler function as the first argument, instead of the flag name.
--

local M = {}

-- unpackNonFlags( args, startPos )
-- Returns arguments from 'args[startPos]' onwards, until next flag.
local function unpackNonFlags( args, startPos )
	local endPos = 0
	for i = startPos, #args do
		local v = args[i]
		if not v or v:sub( 1, 1 ) == '-' then
			break
		else
			endPos = i
		end
	end
	return table.unpack( args, startPos, endPos )
end

function M.process( args, handlers )
	-- gather unprocessed, non-flag parameters in 'leftOvers'
	local leftOvers = {}

	local i = 1
	while args[i] do
		local v = args[i]
		local _, _, flag = v:find( "%-+(.+)" )

		if not flag then
			leftOvers[#leftOvers + 1] = v
		else
			local h = handlers[flag]
			if not h then
				return nil, "Unexpected flag: '" .. v .. "'"
			elseif type( h ) == 'string' then
				-- flag aliasing
				flag = h
				h = handlers[h]
			end

			if type( h ) ~= 'function' then
				error( "Invalid handler for '" .. v .. "':" .. tostring( h ), 0 )
			end

			-- Now, find out how many non-flag parameters we can provide the call
			local n, errorString = h( flag, unpackNonFlags( args, i + 1 ) )
			if errorString then
				return nil, "Error near '" .. v .. "': " .. errorString .. "."
            end

			if n then
				n = tonumber( n )
				if not n then
					error( "Handler for '" .. flag .. "' returned a bad, non-numeric value.", 0 )
				end
				i = i + n  -- skip n
			end
		end
		i= i + 1
	end

	return leftOvers
end

return M
