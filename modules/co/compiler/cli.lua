-------------------------------------------------------------------------------
-- Coral CSL Compiler CLI Program
-------------------------------------------------------------------------------

local debug = require "debug"
local cmdline = require "cmdline"
local compiler = require( "co.compiler" ):new()

-- garbage collection is unnecessary for a compiler
collectgarbage( "stop" )

-------------------------------------------------------------------------------
-- Command-Line Handler Functions
-------------------------------------------------------------------------------

local moduleName = nil
local askedForDox = false
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

function flags.path( flag, dirList )
	if not dirList then
		return nil, "missing comma-separated list of dirs (Coral repositories)"
	end
	co.addPath( dirList )
	return 1
end

function flags.generate( flag, name )
	if not name then return nil, "missing module name" end
	moduleName = name
	return 1
end

function flags.dox()
	askedForDox = true
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

function flags.ignoredupes()
	compiler.ignoreDupesInPath = true
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
  If -g is specified, the compiler will generate source code for a module,
  and the passed list of types will be considered extra module dependencies.
  If --dox is passed along with -g, the compiler will generate documentation
  for a module (as a set of input files for Doxygen), instead of source code.
Available options:
  -p, --path EXTRA,DIRS  Add a list of repositories to the Coral path.
  -g, --generate MODULE  Generate source code for the specified module.
      --dox              Generate input for Doxygen, instead of source code.
  -o, --outdir DIR       Output dir for generated files (default: ./generated).
  -m, --mappingsdir DIR  Separate output dir for mappings (when not specified,
                         mappings are generated in the 'outdir').
      --ignoredupes      Supresses warnings about clashing CSL files.
  -v, --version          Show version information.]]
end

-------------------------------------------------------------------------------
-- Command-Line Processing
-------------------------------------------------------------------------------

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

if askedForDox and not moduleName then
	print( "Error: use -g to specify the module you want to generate documentation for." )
	return -2
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
		if askedForDox then
			compiler:generateDox( moduleName )
		else
			compiler:generateModule( moduleName )
		end
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
