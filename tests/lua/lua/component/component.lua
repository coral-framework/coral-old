require "lua.test"

local M = {}

function M:initialize( module )
	-- Define a new Coral component type in Lua
	local BatComponent = co.Component{
		name = "lua.bat.Component",
		provides = {
			vampireBat = "moduleA.IBat",
			fruitBat = "moduleA.IBat",
			batman = "moduleA.IBatman",
		},
		requires = {
			bat = "moduleA.IBat",
			target1 = "moduleA.IHuman",
			target2 = "moduleA.IHuman",
		},
	}

	ASSERT_EQ( BatComponent.__reflector.type.fullName, "lua.bat.Component" )

	local batInstance = co.new "lua.bat.Component"
	ASSERT_EQ( batInstance.componentType.fullName, "lua.bat.Component" )
end

return M
