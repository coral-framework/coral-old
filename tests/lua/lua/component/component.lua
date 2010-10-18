require "lua.test"

local M = {}

-- Define a new component type in Lua
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

function BatComponent:getBloodsucker()
	return self.bloodsucker or false
end

function BatComponent.vampireBat:getBloodsucker()
	return true
end

function BatComponent:getName()
	return "DefaultName"
end

function BatComponent.batman:getName()
	return "Batman"
end

function BatComponent:fightCrime()
	-- test passing our own interfaces to another component
	local component = self.requiredTarget1.interfaceOwner
	component.bat = self.vampireBat
	component.target1 = self.batman
end

function BatComponent:getRequiredBat() return self.requiredBat end
function BatComponent:setRequiredBat( bat ) self.requiredBat = bat end
function BatComponent:getRequiredTarget1() return self.requiredTarget1 end
function BatComponent:setRequiredTarget1( human ) self.requiredTarget1 = human end

local function testBatInstance( batInstance )
	ASSERT_EQ( batInstance.componentType.fullName, "lua.bat.Component" )
	ASSERT_EQ( batInstance.vampireBat.bloodsucker, true )
	ASSERT_EQ( batInstance.fruitBat.bloodsucker, false )
	ASSERT_EQ( batInstance.batman.bloodsucker, false )

	ASSERT_EQ( batInstance.fruitBat.name, nil )		-- bats don't have a name
	ASSERT_EQ( batInstance.batman.name, "Batman" )	-- only Batman has a name
end

-- Define another component type in Lua
local TestComponent = co.Component{
	name = "lua.test.Component",
	provides = {
		testItf = "moduleA.TestInterface"
	}
}

local testStruct = co.new "moduleA.TestStruct"

function TestComponent.testItf:testOutParameters()
	return 1.25, 'Second', "hello", testStruct, nil, { 1.5, 2.5, 3.5 }, nil
end

function M:initialize( module )
	-- BatComponent tests
	testBatInstance( BatComponent{ bloodsucker = false } )
	ASSERT_ERROR( function() testBatInstance( BatComponent{ bloodsucker = true } ) end, "true != false" )

	local bc = co.new "lua.bat.Component"
	testBatInstance( bc )

	-- test getting & setting the required (client) interfaces
	ASSERT_EQ( nil, bc.bat )
	bc.bat = bc.vampireBat
	ASSERT_EQ( bc.vampireBat, bc.bat )
	ASSERT_EQ( nil, bc.target1 )
	bc.target1 = bc.batman
	ASSERT_EQ( bc.batman, bc.target1 )
	ASSERT_ERROR( function() bc.target1 = bc.fruitBat end, "moduleA.IHuman expected, got moduleA.IBat" )
	ASSERT_ERROR( function() return bc.target2 end, "missing method 'getRequiredTarget2'" )

	-- test passing interfaces from within the component
	local bc2 = BatComponent()
	bc.bat = nil
	bc.target1 = bc2.batman
	ASSERT_EQ( bc.bat, nil )
	ASSERT_EQ( bc.target1, bc2.batman )
	ASSERT_EQ( bc2.bat, nil )
	ASSERT_EQ( bc2.target1, nil )
	bc.batman:fightCrime()
	ASSERT_EQ( bc.bat, nil )
	ASSERT_EQ( bc.target1, bc2.batman )
	ASSERT_EQ( bc2.bat, bc.vampireBat )
	ASSERT_EQ( bc2.target1, bc.batman )

	-- TestComponent tests
	local tc = co.new "lua.test.Component"
	local size, enumValue, text, ts, dummyItf, intList, itfList = tc.testItf:testOutParameters()
	ASSERT_EQ( 1.25, size )
	ASSERT_EQ( 3, #intList )
	ASSERT_EQ( 2, intList[2] )
	ASSERT_EQ( 0, #itfList )
end

return M
