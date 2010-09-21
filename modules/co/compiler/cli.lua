-------------------------------------------------------------------------------
-- Coral CSL Compiler CLI Program
-------------------------------------------------------------------------------

local debug = require "debug"
local cmdline = require "co.compiler.cmdline"
local compiler = require( "co.compiler" ):new()

-------------------------------------------------------------------------------
-- Handling of Command-Line Args
-------------------------------------------------------------------------------

local moduleName = nil
local askedForVersion = false

local flags = {
	-- Aliases
	g = 'generate',
	p = 'path',
	o = 'outdir',
	m = 'mappingsdir',
	v = 'version',
	h = 'help',
}

function flags.generate( flag, name )
	if not name then return nil, "missing module name" end
	moduleName = name
	return 1
end

function flags.path( flag, dirList )
	if not dirList then
		return nil, "missing comma-separated list of dirs (Coral repositories)"
	end
	co.addPath( dirList )
	return 1
end

function flags.outdir( flag, dir )
	if not dir then return nil, "missing output directory" end
	compiler.outDir = dir
	return 1
end

function flags.mappingsdir( flag, dir )
	if not dir then return nil, "missing mappings directory" end
	mappingsDir = dir
	return 1
end

function flags.version()
	print( "Coral Compiler v" .. co.version .. " (" .. co.buildKey .. ")" )
	askedForVersion = true
end

function flags.help()
	flags.version()
	print [[
Usage: coralc [options] [-g, --generate MODULE] [TYPE] ...
Description:
  Generates mappings for the list of types passed as command-line arguments.
  If the -g option is specified (alongside a module name), the compiler will
  generate code for a module. In this case, the passed list of types will be
  considered extra module dependencies.
Available options:
  -p, --path EXTRA,DIRS  Add a list of repositories to the Coral path.
  -o, --outdir DIR       Output dir for generated files (default: ./generated).
  -m, --mappingsdir DIR  Separate output dir for mappings (when not specified,
                         mappings are generated in the 'outdir').
  -v, --version          Show version information.]]
end

local allArgs = {...}
if #allArgs == 0 then
	flags.help()
	return 0
end

local args, errorString = cmdline.process( allArgs, flags )
if not args then
	print( errorString )
	return -2
end

if askedForVersion then
	return 0
end

-------------------------------------------------------------------------------
-- Main
-------------------------------------------------------------------------------

compiler.log = print

local ok, err = xpcall( function()
	for i = 1, #args do
		compiler:addType( args[i] )
	end

	if moduleName then
		compiler:generateModule( moduleName )
	else
		compiler:generateMappings()
	end
end, debug.traceback )

if not ok then
	print( "*** Error ***" )
	print( err )
	return -1
end

return 0
