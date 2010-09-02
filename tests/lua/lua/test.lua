function ASSERT_TRUE( v )
	if not v then
		error( "ASSERT_TRUE failed (" .. tostring( v ) .. " value)", 2 )
	end
end

function ASSERT_EQ( v1, v2 )
	if v1 ~= v2 then
		error( "ASSERT_EQ failed: " .. tostring( v1 ) .. " != " .. tostring( v2 ), 2 )
	end
end

function ASSERT_ERROR( func, expectedErrorMsg )
	local ok, msg = pcall( func )
	if not ok then
		if expectedErrorMsg and ( msg ~= expectedErrorMsg and not msg:find( expectedErrorMsg ) ) then
			error( "ASSERT_ERROR failed: raised error ('" .. msg .. "') does not contain the expected message ('"
					.. expectedErrorMsg .. "')", 2 )
		end
	else
		error( "ASSERT_ERROR failed: no error was raised, though " .. ( not expectedErrorMsg and "one was expected" or
			( "the following was expected: '" .. expectedErrorMsg .. "'" ) ), 2 )
	end
end

-- pretty( caption, var ): pretty prints a value, properly dealing with tables and cycles.
-- Useful for debugging. To print a var, call pretty( "var", var ).

local function printTypesIn( ns )
	print( "Types in namespace '" .. ns.fullName .. "':" )

	local types = ns.types
	for i = 1, #types do
		print( types[i].fullName )
	end

	local children = ns.childNamespaces
	for i = 1, #children do
		printTypesIn( children[i] )
	end
end

local function displayValue( value )
	if type( value ) == 'string' then
		return string.gsub( string.format( '%q', value ), '^"([^"\']*)"$' , "'%1'" )
	else
		return tostring( value )
	end
end

local function asKey( value, separator )
	if type( value ) == 'string' and string.find( value, '^[%w_]+$' ) then
		return separator .. value
	end
	return '[' .. displayValue( value ) .. ']'
end

local function printHorizontalArray( t, maxLength )
	local res, sep = '', ''
	for i = 1, #t do
		if type( t[i] ) == 'table' then
			return
		end
		res = res .. sep .. displayValue( t[i] )
		if #res > maxLength then
			return
		end
		sep = ','
	end
	return '(' .. res .. ')'
end

local function printHorizontalMap( t, maxLength )
	local res, sep = '', ''
	for k, v in pairs( t ) do
		if type( v ) == 'table' then
			return
		end
		res = res .. sep .. asKey( k, '' ) .. '=' .. displayValue( v )
		if #res > maxLength then
			return
		end
		sep = ','
	end
	return '{' .. res .. '}'
end

function pretty( printKey, value, cache, realKey )
	if not printKey then
		printKey, value = '_G', _G
	end
	if type( value ) == 'table' then
		if not cache then
			cache = {}
		end
		if cache[value] then
			value = cache[value]
		else
			if not realKey then
				realKey = printKey
			end
			cache[value] = realKey
			local listOfKeys = {}
			for k, _ in pairs( value ) do
				listOfKeys[#listOfKeys + 1] = k
			end
			if #listOfKeys == 0 then
				value = '{}'
			else
				local maxLength = 75 - #printKey
				local res = ( #listOfKeys == #value and printHorizontalArray( value, maxLength ) )
				if not res then
					res = printHorizontalMap( value, maxLength )
				end
				if not res then
					table.sort( listOfKeys )
					for i = 1, #listOfKeys do
						if listOfKeys[i] then
							local suffix = asKey( listOfKeys[i], '.' )
							pretty( printKey .. suffix, value[listOfKeys[i]], cache, realKey .. suffix )
							printKey = string.rep( ' ', #printKey )
						end
					end
					return
				end
				value = res
			end
		end
	else
		value = displayValue( value )
	end
	print( printKey .. ' = ' .. value )
end
