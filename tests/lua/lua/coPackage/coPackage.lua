require "lua.test"

local M = {}

function M:initialize( module )
	-- test the co.system interface
	ASSERT_TRUE( co.system )
	ASSERT_EQ( co.system.interfaceType.fullName, "co.System" )

	-- test the co.getType() function
	ASSERT_EQ( co.getType( "bool" ).fullName, "bool" )
	ASSERT_EQ( co.getType( "co.ArrayType" ).name, "ArrayType" )
	ASSERT_ERROR( function() co.getType( "nonExistingType" ) end, "error loading type 'nonExistingType'" )

	-- test the co.new() function
	local testComponent = co.new( "moduleA.TestComponent" )
	ASSERT_EQ( testComponent.componentType.fullName, "moduleA.TestComponent" )

	local testStruct = co.new( "moduleA.TestStruct" )
	ASSERT_EQ( testStruct.anInt8, 0 )

	local testNativeClass = co.new "moduleA.Vec2D"
	ASSERT_EQ( testNativeClass.x, 0 )
	ASSERT_EQ( testNativeClass.y, 0 )

	ASSERT_ERROR( function() co.new "co.Type" end, "co.Type is not instantiable" )
	ASSERT_ERROR( function() co.new "co.SystemState" end, "co.SystemState is not instantiable" )
	ASSERT_ERROR( function() co.new "int8" end, "int8 is not instantiable" )
	ASSERT_ERROR( function() co.new "nonExistingType" end, "error loading type 'nonExistingType'" )
end

return M
