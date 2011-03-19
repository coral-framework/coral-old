require "lua.test"

local M = {}

function M:initialize( module )
	-- test the co.system interface
	ASSERT_TRUE( co.system )
	ASSERT_EQ( co.system.interfaceType.fullName, "co.ISystem" )

	-- test the co.getType() function
	ASSERT_EQ( co.getType( "bool" ).fullName, "bool" )
	ASSERT_EQ( co.getType( "co.IArrayType" ).name, "IArrayType" )
	ASSERT_ERROR( function() co.getType( "nonExistingType" ) end, "could not load type 'nonExistingType'" )

	-- test the co.Type table
	ASSERT_EQ( co.Type "bool", co.getType( "bool" ) )
	ASSERT_EQ( co.Type["co.IArrayType"], co.getType( "co.IArrayType" ) )
	ASSERT_ERROR( function() return co.Type.nonExistingType end, "could not load type 'nonExistingType'" )

	-- test the co.new() function
	local testComponent = co.new( "moduleA.TestComponent" )
	ASSERT_EQ( testComponent.componentType.fullName, "moduleA.TestComponent" )

	local testStruct = co.new( "moduleA.TestStruct" )
	ASSERT_EQ( testStruct.anInt8, 0 )

	local testNativeClass = co.new "moduleA.Vec2D"
	ASSERT_EQ( testNativeClass.x, 0 )
	ASSERT_EQ( testNativeClass.y, 0 )

	ASSERT_ERROR( function() co.new "co.IType" end, "co.IType is not instantiable" )
	ASSERT_ERROR( function() co.new "co.SystemState" end, "co.SystemState is not instantiable" )
	ASSERT_ERROR( function() co.new "int8" end, "int8 is not instantiable" )
	ASSERT_ERROR( function() co.new "nonExistingType" end, "could not load type 'nonExistingType'" )

	-- test findScript()
	ASSERT_TRUE( co.findScript( "lua.coPackage.coPackage" ) )
	ASSERT_TRUE( co.findScript( "lua.coPackage.nonExistingScript" ) == nil )

	-- test findModuleFile
	ASSERT_TRUE( co.findModuleFile( "lua.coPackage", "coPackage.lua" ) )
	ASSERT_TRUE( co.findModuleFile( "lua.coPackage", "nonExisting.lua" ) == nil )
end

return M
