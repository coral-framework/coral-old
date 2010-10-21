-- printVar( caption, var ): prints a variable, properly dealing with tables and cycles.
-- Useful for debugging. To print a var, call printVar( "Var", var ). You may also specify
-- your own print function as the first argument: printVar( myPrint, "Var", var ).

local type = type
local tostring = tostring
local rep = string.rep
local gsub = string.gsub
local find = string.find
local format = string.format

local function formatValue( var )
	if type( var ) == 'string' then
		return gsub( format( '%q', var ), '^"([^"\']*)"$' , "'%1'" )
	else
		return tostring( var )
	end
end

local function asKey( var, separator )
	if type( var ) == 'string' and find( var, '^[%w_]+$' ) then
		return separator .. var
	end
	return '[' .. formatValue( var ) .. ']'
end

local function formatHorizontalArray( t, maxLength )
	local res, sep = '', ''
	for i = 1, #t do
		if type( t[i] ) == 'table' then
			return
		end
		res = res .. sep .. formatValue( t[i] )
		if #res > maxLength then
			return
		end
		sep = ','
	end
	return '(' .. res .. ')'
end

local function formatHorizontalMap( t, maxLength )
	local res, sep = '', ''
	for k, v in pairs( t ) do
		if type( v ) == 'table' then
			return
		end
		res = res .. sep .. asKey( k, '' ) .. '=' .. formatValue( v )
		if #res > maxLength then
			return
		end
		sep = ','
	end
	return '{' .. res .. '}'
end

local function printVar( printer, caption, var, cache, key )
	if not printer then
		printer, caption, var = print, '_G', _G
	elseif not var then
		if type( printer ) ~= 'function' then
			printer, caption, var = print, printer, caption
		end
	end
	if type( var ) == 'table' then
		if not cache then
			cache = {}
		end
		if cache[var] then
			var = cache[var]
		else
			if not key then
				key = caption
			end
			cache[var] = key
			local listOfKeys = {}
			for k, _ in pairs( var ) do
				listOfKeys[#listOfKeys + 1] = k
			end
			if #listOfKeys == 0 then
				var = '{}'
			else
				local maxLength = 75 - #caption
				local res = ( #listOfKeys == #var and formatHorizontalArray( var, maxLength ) )
				if not res then
					res = formatHorizontalMap( var, maxLength )
				end
				if not res then
					table.sort( listOfKeys )
					for i = 1, #listOfKeys do
						if listOfKeys[i] then
							local suffix = asKey( listOfKeys[i], '.' )
							printVar( printer, caption .. suffix, var[listOfKeys[i]], cache, key .. suffix )
							caption = rep( ' ', #caption )
						end
					end
					return
				end
				var = res
			end
		end
	else
		var = formatValue( var )
	end
	printer( caption .. ' = ' .. var )
end

return printVar
