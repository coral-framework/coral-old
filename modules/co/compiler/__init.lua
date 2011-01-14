-------------------------------------------------------------------------------
-- Coral CSL Compiler Module
-------------------------------------------------------------------------------

local lfs = require "lfs"
local path = require "path"
local utils = require "co.compiler.utils"
local dependencies = require "co.compiler.dependencies"
local TypeWrapper = require "co.compiler.TypeWrapper"

local dox = require "co.compiler.dox"
local doxyConf = require "co.compiler.dox.conf"
local moduleDox = require "co.compiler.dox.module"

local mapping = require "co.compiler.mapping"

local componentBaseHeader = require "co.compiler.module.componentBaseHeader"
local componentBaseSource = require "co.compiler.module.componentBaseSource"
local nativeClassAdapter = require "co.compiler.module.nativeClassAdapter"
local reflector = require "co.compiler.module.reflector"

local moduleBootstrap = require "co.compiler.module.bootstrap"
local moduleInstallerHeader = require "co.compiler.module.installerHeader"
local moduleInstallerSource = require "co.compiler.module.installerSource"
local moduleDefaultPart = require "co.compiler.module.defaultPart"

-- Compiler Class
local Compiler = {
	------ Default Field Values --
	outDir = "./generated",		-- output dir for generated files
	log = function( ... ) end,	-- function called to print misc. info (stats, etc.)

	------ Easy access to re-usable modules/templates ------
	utils = require "co.compiler.utils",
	header = require "co.compiler.header",
	revisionCheck = require "co.compiler.module.revisionCheck",
}

Compiler.__index = Compiler

-- Creates a new Compiler instance.
function Compiler:new()
	local self = setmetatable( {}, Compiler )

	-- array of module types
	self.types = {}

	-- map of direct type dependencies for the module
	-- includes all module types and all explicitly-specified dependencies
	-- these are the types for which we should generate mappings
	self.dependencies = {} -- map[type] = distance

	return self
end

-- Add a type whose mapping should be generated by the next call to generateMappings().
-- It will also be considered a module dependency, if we're generating a module.
function Compiler:addType( typeName )
	local t = TypeWrapper:wrap( co.Type[typeName] )
	self.dependencies[t] = 1
end

local numExpandedFiles = 0

-- Creates a dir hierarchy, opens a file and writes the given template, passing it 'data'.
local function expand( dir, filename, template, ... )
	path.makePath( dir )
	local f = io.open( dir .. '/' .. filename, "w" )
	template( function( ... ) f:write( ... ) end, ... )
	f:close()
	numExpandedFiles = numExpandedFiles + 1
end

-- Runs the compiler to generate documentation. Each module type produces a reshaped CSL
-- file for consumption by Doxygen, containing the type's description and documentation.
function Compiler:generateDox( moduleName )
	assert( not self.updatedTypes and #self.types == 0, "this compiler instance has already been used" )
	self.moduleName = moduleName
	self:loadModuleTypes()
	numExpandedFiles = 0
	for i, t in ipairs( self.types ) do
		expand( self.outDir, t.name .. '.dox', dox, self, t )
	end
	expand( self.outDir, '__module.dox', moduleDox, self )
	expand( self.outDir, 'Doxygen.conf', doxyConf, self )
	self.log( "Generated " .. numExpandedFiles .. " documentation files." )
end

local function isBuiltInType( t )
	local name = t.fullName
	return name == 'co.Interface' or name == 'co.TypeKind'
end

local function reportChanges( action, subject, changed, total )
	if changed == 0 then
		return "All " .. total .. " " .. subject .. "s are up to date."
	else
		local msg = action .. " " .. changed .. " " .. subject .. ( changed > 1 and 's' or '' )
		if changed < total then
			local numChanged = ( total - changed )
			msg = msg .. " (" .. numChanged .. ( numChanged > 1 and " were" or " was" ) .. " up to date)"
		end
		return msg .. "."
	end
end

-- Runs the compiler to generate mappings. Use this when you don't need to generate a module.
function Compiler:generateMappings()
	assert( not self.updatedTypes, "this compiler instance has already been used" )

	if not self.mappingsDir then
		self.mappingsDir = self.outDir
	end

	-- if not called through generateModule(), we must load the cache ourselves
	if not self.moduleName then
		self:loadCache()
	end

	local cachedTypes = self.cachedTypes
	local updatedTypes = {}
	local numMappings = 0
	numExpandedFiles = 0

	-- generate mappings for all direct and indirect dependencies
	for t, dist in dependencies( self.dependencies ) do
		-- we may find additional direct module dependencies
		if dist <= 1 then self.dependencies[t] = dist end
		-- only create mappings for non-component, non-built-in types
		if t.kind ~= 'TK_COMPONENT' and not isBuiltInType( t ) then
			numMappings = numMappings + 1
			-- only regenerate out-of-date files
			if cachedTypes[t.fullName] ~= t.fullSignatureStr then
				local dir = self.mappingsDir .. '/' .. t.namespace.fullName:gsub( '%.', '/' )
				expand( dir, t.name .. '.h', mapping, self, t )
				updatedTypes[t.fullName] = t.fullSignatureStr
			end
		end
	end

	self.updatedTypes = updatedTypes

	self.log( reportChanges( "Created", "mapping", numExpandedFiles, numMappings ) )

	if not self.moduleName then
		self:saveCache()
	end
end

-- Runs the compiler to generate a module. Also generates the necessary mappings.
function Compiler:generateModule( moduleName )
	assert( self.moduleName == nil, "this compiler instance has already been used" )

	self.moduleName = moduleName
	self.moduleNS = self.utils.toCppName( moduleName )

	self:loadCache()
	self:loadModuleTypes()

	local doing = ( next( self.cachedTypes ) and "Updating" or "Generating" )
	self.log(  doing .. " code for module '" .. self.moduleName .. "' (" .. #self.types .. " types)..." )

	-- add types that may not come as explicit dependencies, but are required by generated module code
	self:addType( "co.System" )
	self:addType( "co.Namespace" )
	self:addType( "co.Reflector" )
	self:addType( "co.ModulePart" )
	--  all type classes are implicit dependencies
	self:addType( "co.EnumType" )
	self:addType( "co.ArrayType" )
	self:addType( "co.StructType" )
	self:addType( "co.ExceptionType" )
	self:addType( "co.InterfaceType" )
	self:addType( "co.ComponentType" )
	self:addType( "co.NativeClassType" )

	-- generateMappings() also adds entries to self.dependencies
	self:generateMappings()

	local cachedTypes = self.cachedTypes
	local updatedTypes = self.updatedTypes
	local numFiles = 0
	numExpandedFiles = 0

	local function cachedExpand( ... ) numFiles = numFiles + 1 end
	local function updateExpand( dir, filename, template, c, t )
		numFiles = numFiles + 1
		if t then
			updatedTypes[t.fullName] = t.fullSignatureStr
		end
		expand( dir, filename, template, c, t )
	end

	-- Generate per-type files
	for i, t in ipairs( self.types ) do
		-- only regenerate out-of-date files
		local expand = ( cachedTypes[t.fullName] == t.fullSignatureStr and cachedExpand or updateExpand )
		if t.kind == 'TK_NATIVECLASS' then
			expand( self.outDir, t.name .. "_Adapter.h", nativeClassAdapter, self, t )
		elseif t.kind == 'TK_COMPONENT' then
			expand( self.outDir, t.name .. "_Base.h", componentBaseHeader, self, t )
			expand( self.outDir, t.name .. "_Base.cpp", componentBaseSource, self, t )
		end
		expand( self.outDir, t.name .. "_Reflector.cpp", reflector, self, t )
	end

	-- Generate per-module files (only if at least one file was out of date)
	local shouldUpdateModuleFiles = ( numExpandedFiles > 0 or self.cachedNumModuleFiles > #self.types )
	local expand = ( shouldUpdateModuleFiles and updateExpand or cachedExpand )
	expand( self.outDir, "__Bootstrap.cpp", moduleBootstrap, self )
	expand( self.outDir, "ModuleInstaller.h", moduleInstallerHeader, self )
	expand( self.outDir, "ModuleInstaller.cpp", moduleInstallerSource, self )
	if not self:hasCustomModulePart() then
		expand( self.outDir, "__ModulePart.cpp", moduleDefaultPart, self )
	end

	self.log( reportChanges( "Generated", "module file", numExpandedFiles, numFiles ) )

	self:saveCache()
end

-- Loads all module types by locating CSL files in the module's namespace
-- (which may be scattered in several dirs in the Coral path).
function Compiler:loadModuleTypes()
	assert( #self.types == 0 )
	local moduleDirPath = self.moduleName:gsub( '%.', '/' )
	local coralPaths = co.getPaths()
	for i, repositoryDir in ipairs( coralPaths ) do
		local moduleDir = repositoryDir .. '/' .. moduleDirPath
		if path.isDir( moduleDir ) then
			for filename in lfs.dir( moduleDir ) do
				local typeName = filename:match( "(.+)%.csl$" )
				if typeName then
					local moduleType = TypeWrapper:wrap( co.Type[self.moduleName .. '.' .. typeName] )
					if self.dependencies[moduleType] == 0 then
						local msg = "Type '" .. moduleType.fullName .. "' has a duplicate definition in "
									.. "one of the following dirs:\n"
						for k = 1, i do
							msg = msg .. "  [" .. k .. "] " .. coralPaths[k] .. "/" .. moduleDirPath .. "\n"
						end
						error( msg, 0 )
					end
					self.types[#self.types + 1] = moduleType
					self.dependencies[moduleType] = 0
				end
			end
		end
	end
end

-- Returns whether the module has a valid custom co.ModulePart.
function Compiler:hasCustomModulePart()
	local localModuleName = self.moduleName:match( '.-([^%.]+)$' )
	local t = co.system.types:findType( self.moduleName .. '.' .. localModuleName )
	local modulePartItf = co.Type "co.ModulePart"
	if t and t.kind == 'TK_COMPONENT' then
		for i, facet in ipairs( t.facets ) do
			if facet.type:isSubTypeOf( modulePartItf ) then
				if facet.name == 'part' then
					return true
				end
				error( "Module '" .. self.moduleName .. "' contains a component named '" ..
					self.moduleName .. "', but its co.ModulePart facet is not named 'part'.", 0  )
			end
		end
		error( "Module '" .. self.moduleName .. "' contains a component named '" ..
			self.moduleName .. "', but it does not provide the co.ModulePart interface.", 0  )
	end
	return false
end

local function loadCacheFile( filename, cachedTypes )
	local f = io.open( filename, 'r' )
	local code = f:read( "*a" )
	f:close()
	local chunk, err = loadin( cachedTypes, code, filename, 't' )
	if not chunk then
		return print( "Error in cache file '" .. filename .. "': " .. tostring( err ) )
	end
	local ok, moreCachedTypes, numModuleFiles = pcall( chunk )
	if not ok then
		return print( "Error in cache file '" .. filename .. "': " .. tostring( moreCachedTypes ) )
	end
	if type( moreCachedTypes ) ~= 'table' then
		return print( "Cache file '" .. filename .. "' returned an invalid result (" .. tostring( moreCachedTypes ) .. ")." )
	end
	for k, v in pairs( moreCachedTypes ) do
		local current = cachedTypes[k]
		if current then
			if current ~= v then
				cachedTypes[k] = 'collision' -- handle collisions by forcing an update
			end
		else
			cachedTypes[k] = v
		end
	end
	return numModuleFiles
end

local function saveCacheFile( filename, cachedTypes, numModuleFiles )
	local f = io.open( filename, "w" )
	f:write( [[
-------------------------------------------------------------------------------
-- Coral Compiler v]], co.version, [[ Cache File
-- Generated on ]], utils.currentDateTime(), "\n", [[
-- Delete this file if you want to force all files to be regenerated.
-------------------------------------------------------------------------------

return {
]] )
	for k, v in pairs( cachedTypes ) do
		f:write( '\t["', k, '"] = "', v, '",\n' )
	end
	f:write( "}, ", numModuleFiles, "\n" )
	f:close()
end

local CACHE_FILE_NAME = '/__coralc_cache.lua'

function Compiler:loadCache()
	local cachedTypes = {}
	local cachedNumModuleFiles

	-- if one of the standard, compiler-generated files is missing, ignore the cache files and
	-- force a full update; this is a workaround for IDEs with inflexible 'clean' routines (e.g. MSVC)
	-- which delete most of the generated source files, but not the cache files.
	local ignoreCache = (
			( self.mappingsDir and not path.isFile( self.mappingsDir .. '/co/System.h' ) ) or
			( self.moduleName and not path.isFile( self.outDir .. '/__Bootstrap.cpp' ) )
		)

	if not ignoreCache then
		local filename = self.outDir .. CACHE_FILE_NAME
		if path.isFile( filename ) then
			cachedNumModuleFiles = loadCacheFile( filename, cachedTypes )
		end
		if self.mappingsDir and self.mappingsDir ~= self.outDir then
			filename = self.mappingsDir .. CACHE_FILE_NAME
			if path.isFile( filename ) then
				loadCacheFile( filename, cachedTypes )
			end
		end
	end

	self.cachedTypes = cachedTypes
	self.cachedNumModuleFiles = cachedNumModuleFiles or 0
end

function Compiler:saveCache()
	local cachedTypes = self.cachedTypes
	local updatedTypes = self.updatedTypes
	assert( type( cachedTypes ) == 'table' )
	-- merge 'updatedTypes' into 'cachedTypes'
	for k, v in pairs( updatedTypes ) do
		cachedTypes[k] = v
	end
	saveCacheFile( self.outDir .. CACHE_FILE_NAME, self.cachedTypes, #self.types )
	if self.mappingsDir ~= self.outDir then
		saveCacheFile( self.mappingsDir .. CACHE_FILE_NAME, self.cachedTypes, 0 )
	end
end

return Compiler
