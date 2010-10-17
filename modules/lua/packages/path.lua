-------------------------------------------------------------------------------
-- Module for portable path/directory operations.
-- Requires LuaFileSystem.
-------------------------------------------------------------------------------

local lfs = require "lfs"
local attrib = lfs.attributes

local DIR_SEP = package.config:sub( 1, 1 )
local isWindows = ( DIR_SEP == '\\' )

-- Returns whether a path is (one of) the system's root path.
local function isRoot( path )
	if isWindows then
		return #path <= 3 and path:sub( 2, 1 ) == ':'
	else
		return path == '/'
	end
end

-- Returns whether a path is absolute (i.e. starts with a root path).
local function isAbs( path )
	if isWindows then
		return path:sub( 2, 1 ) == ':'
	else
		return path:sub( 1, 1 ) == '/'
	end
end

-- Returns whether path is a file.
local function isDir( path )
	return attrib( path, 'mode' ) == 'directory'
end

-- Returns whether a path is a directory.
local function isFile( path )
	return attrib( path, 'mode' ) == 'file'
end

-- Returns whether a path exists (be a dir, a file, or whatever).
local function exists( path )
	return attrib( path, 'mode' ) ~= nil
end

-- Returns a normalized path with the following transformations:
-- 	 * On Windows, converts directory separators to '/' (except for the root drive separator, i.e. "c:\").
--   * On UNIX, reduces "/./" to "/".
--	 * Reduces "/dir/../" to "/".
--   * Reduces "//" to "/".
--   * Removes trailing '/'.
local function normalize( path )
	-- add a temporary trailing slash to facilitate the transformations
	path = path .. '/'

	-- system-specific bits
	if isWindows then
		path = path:gsub( '\\' , '/' )
		path = path:gsub( '^(%a):/', '%1:\\' )
	else
		path = path:gsub( '/%./', '/' )
	end

	-- all "//" to "/"
	path:gsub( '//', '/' )

	-- all "/dir/../" to "/"
	path:gsub( '/[^/]+/%.%./', '/' )

	-- remove trailing slash
	return path:sub( 1, -2 )
end

-- Creates a directory hierarchy.
local function _makePath( path )
	if isRoot( path ) or isDir( path ) then
		return
	end
	_makePath( path:match( '(.+)/[^/]+$' ) )
	if not lfs.mkdir( path ) then
		error( "error creating directory '" .. path .. "'" )
	end
end

local function makePath( path )
	_makePath( normalize( path ) )
end

local M = {
	isRoot = isRoot,
	isAbs = isAbs,
	isDir = isDir,
	isFile = isFile,
	exists = exists,
	normalize = normalize,
	makePath = makePath,
}

return M
