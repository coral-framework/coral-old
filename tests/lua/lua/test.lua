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
