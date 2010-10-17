-------------------------------------------------------------------------------
-- Lua module's main package
-------------------------------------------------------------------------------

-- localize commonly used functions
local error = error
local pairs = pairs
local ipairs = ipairs
local tostring = tostring
local getmetatable = getmetatable
local setmetatable = setmetatable

local coNew = co.new
local coGetType = co.getType
local coFindScript = co.findScript
local coRootNS = co.system.types.rootNS
local coNewComponentType = co.newComponentType
local coNewComponentInstance = co.newComponentInstance

-------------------------------------------------------------------------------
-- Because this module script is manually executed, it must register itself.

package.loaded.lua = true

-------------------------------------------------------------------------------
-- Set the Lua search path to our 'lua/packages' dir.

local thisScriptFileName = ...
local thisModuleDir = thisScriptFileName:match( "(.+)__init%.lua$" )
local luaPackagesDir = thisModuleDir .. 'packages'
package.path = luaPackagesDir .. "/?.lua;" .. luaPackagesDir .. "/?/init.lua"

-------------------------------------------------------------------------------
-- Install our own loader function into Lua's package.loaders table.

table.insert( package.loaders, 2, function( moduleName )
	local filename = coFindScript( moduleName )
	if filename then
		return loadfile( filename )
	end
end )

-------------------------------------------------------------------------------
-- type = co.Type[typeName]
--
-- co.Type is a smart table that fetches and caches Coral types. It can be
-- indexed or called, always with a fully-qualified type name. For example:
--		local type = co.Type[fullTypeName]
--		local type = co.Type "co.Interface"

local coTypeMT = {}

function coTypeMT.__index( t, typeName )
	local res = coGetType( typeName )
	t[typeName] = res
	return res
end

function coTypeMT.__call( t, typeName )
	return t[typeName]
end

local coType = setmetatable( {}, coTypeMT )
co.Type = coType

-------------------------------------------------------------------------------
-- componentPrototype = co.Component( desc )
--
-- Defines a Coral component type implemented in Lua.
--
-- Receives a table describing the component type, with three fields:
--		desc.name = a fully-qualified Coral type name for the component.
--		desc.provides = a map of interface names to interface type names.
--		desc.requires = a map of interface names to interface type names.
-- Either 'provides' or 'requires' can be omitted, but at least one interface
-- must be defined. When providing both tables, beware of interface name clashes.
--
-- The 'desc' table is converted into a component prototype table and returned
-- as the function result. A brand new component prototype table contains only
-- the component's provided (server) interfaces, as interface prototype tables.
--
-- Prototype tables should be populated with Lua methods to implement the component.
-- A method will only affect a specific server interface if it is added to the
-- interface's prototype table; conversely, it will affect all interfaces if it
-- is added to the component's prototype table. For example:
--		local MyComponent = co.Component{ ... }
--		function MyComponent:doSomething() print "all interfaces" end
--		function MyComponent.someItf:doSomething() print "just someItf" end
--
-- A component can be instantiated by calling the component's prototype table. For example:
--		local instance = MyComponent() -- either no args, or...
--		local instance = MyComponent{} -- a table, which is re-used as the component instance.
--		local instance = MyComponent{ initial = 1, values = 2, areOK = 3 }
--
-- It is also possible to instantiate a Lua component from C++ by using the component's
-- type reflector. Therefore, co::newInstance() should also work for Lua components.

local function checkComponentInterfaces( provides, requires )
	local ok = false

	-- turn type names into type instances
	if provides then
		for k, v in pairs( provides ) do
			ok = true
			local t = coType[v]
			local r = t.reflector -- force reflectors to be loaded
			provides[k] = t
		end
	end

	if requires then
		for k, v in pairs( requires ) do
			ok = true
			requires[k] = coType[v]
			if provides and provides[k] then
				error( "required interface '" .. k .. "' clashes with a provided interface" )
			end
		end
	end

	if not ok then
		error( "no interface was specified" )
	end
end

local function defineComponentType( ns, typeName, tct, provides, requires )
	local typeBuilder = ns:defineType( typeName, 'TK_COMPONENT', tct )

	if provides then
		for itfName, itfType in pairs( provides ) do
			typeBuilder:defineInterface( itfName, itfType, true )
		end
	end

	if requires then
		for itfName, itfType in pairs( requires ) do
			typeBuilder:defineInterface( itfName, itfType, false )
		end
	end

	return typeBuilder:createType()
end

local ComponentMT = {}

function co.Component( t )
	local fullName = t.name
	if not fullName then
		error( "you must specify a component name", 2 )
	end

	local provides = t.provides
	local requires = t.requires
	local ok, err = pcall( checkComponentInterfaces, provides, requires )
	if not ok then
		error( "invalid component interfaces: " .. tostring( err ), 2 )
	end

	local nsName, typeName = fullName:match( '(.+)%.([^%.]+)$' )
	if not typeName then
		nsName = ''
		typeName = fullName
	end

	local ns = coRootNS
	for name in nsName:gmatch( "([^%.]+)%.?" ) do
		ns = ns:getChildNamespace( name ) or ns:defineChildNamespace( name )
	end

	if ns:getType( typeName ) then
		error( "component name '" .. fullName .. "' clashes with an existing type", 2 )
	end

	local tct = coNew( "co.TypeCreationTransactionComponent" ).transaction
	local ok, ct = pcall( defineComponentType, ns, typeName, tct, provides, requires )
	if ok then
		tct:commit()
	else
		tct:rollback()
		error( "could not define a new component type: " .. tostring( ct ), 2 )
	end

	-- convert 't' into a component prototype table
	t.name = nil
	t.provides = nil
	t.requires = nil

	t.__provides = provides
	t.__requires = requires
	t.__reflector = coNewComponentType( ct, t )

	-- a component prototype table is the MT for its component instances
	t.__index = function( _, k ) return t[k] end

	-- create prototype tables for the server interfaces
	for k, v in pairs( provides ) do
		t[k] = {}
	end

	return setmetatable( t, ComponentMT )
end

local function interfaceMT__index( itfTable, k )
	return itfTable.__protoItf[k] or getmetatable( itfTable )[k]
end

function ComponentMT.__call( protoTable, instanceTable )
	instanceTable = instanceTable or {}
	instanceTable.__index = interfaceMT__index

	local instance = coNewComponentInstance( protoTable.__reflector, instanceTable )

	-- setup the server interface instance tables
	for k, v in pairs( protoTable.__provides ) do
		instanceTable[k] = setmetatable( { __interface = instance[k], __protoItf = protoTable[k] }, instanceTable )
	end

	setmetatable( instanceTable, protoTable )

	return instance
end
