require "lua.test"

local M = {}

function M:initialize( module )
	-- printTypesIn( co.system.types.rootNS )

	-- test the number of types in the l
	local luaModuleTypes = co.system.types:findNamespace( "lua" ).types
	ASSERT_EQ( 8, #luaModuleTypes )
	ASSERT_EQ( "Exception", luaModuleTypes[1].name )
	ASSERT_EQ( "lua", luaModuleTypes[8].name )

	-- test a method that returns an array of structs as an output param
	local type, cslErrors = co.system.types:loadType( "TypeLoaderTests.NestedErrors.Struct1" )
	ASSERT_EQ( nil, type )
	ASSERT_EQ( 5, #cslErrors )
	--pretty( "cslErrors", cslErrors )
end

return M