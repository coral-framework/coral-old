require "lua.test"

local M = {}

function M:initialize( module )
	-- create a native class instance
	local vec2d = co.new( "moduleA.Vec2D" )

	-- test getting/setting some attributes
	ASSERT_EQ( vec2d.x, 0 )
	ASSERT_EQ( vec2d.y, 0 )
	ASSERT_EQ( vec2d.length, 0 )
	ASSERT_EQ( vec2d.length2, 0 )

	vec2d.x = 1
	ASSERT_EQ( vec2d.x, 1 )
	vec2d.y = 2
	ASSERT_EQ( vec2d.y, 2 )

	ASSERT_EQ( vec2d.length, math.sqrt( 5 ) )
	ASSERT_EQ( vec2d.length2, 5 )

	-- test setting the read-only 'length' attribute
	ASSERT_ERROR( function() vec2d.length = 2 end, "attribute 'length' is read-only and cannot be changed" )

	-- test getting a non-existing attribute
	ASSERT_ERROR( function() return vec2d.dummy end, "non-existing member 'dummy'" )

	-- test the get/set methods
	vec2d:set( 3, 7 )
	ASSERT_EQ( vec2d.x, 3 )
	ASSERT_EQ( vec2d.y, 7 )

	local x, y = vec2d:get()
	ASSERT_TRUE( x == 3 and y == 7 )

	-- test dot()
	local anotherVec2d = co.new( "moduleA.Vec2D" )
	anotherVec2d:set( 2, 2 )

	ASSERT_EQ( vec2d:dot( anotherVec2d ), 20 )

	-- test inout/out native class params
	local s, a, b = co.new "moduleA.Vec2D", co.new "moduleA.Vec2D", co.new "moduleA.Vec2D"
	s:set( 1, 1 )
	a:set( 2, 2 )
	b:set( 3, 3 )

	local r1, r2, r3 = s:testInOut( a, b )

	-- returned values should be new values
	ASSERT_TRUE( r1 ~= s and r2 ~= a and r3 ~= b )

	-- instance 'b' should have been modified
	ASSERT_TRUE( b:equals( a ) )

	-- check returned values
	ASSERT_EQ( r1.x, 1 ) ASSERT_EQ( r1.y, 1 )
	ASSERT_EQ( r2.x, 2 ) ASSERT_EQ( r2.y, 2 )
	ASSERT_EQ( r3.x, 3 ) ASSERT_EQ( r3.y, 3 )
end

return M
