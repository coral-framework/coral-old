/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "Dir.h"
#include "FileSystem.h"
#include "StringTokenizer.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>

#if defined(CORAL_CC_MSVC)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <io.h>
	#include <direct.h>
#elif defined(CORAL_CC_GNU)
	#include <dirent.h>
#else
	#error Unsupported compiler or platform!
#endif

namespace co {

#ifdef CORAL_OS_WIN

// private class used to iterate over a directory
class DirIterator
{
public:
	DirIterator( const std::string& dirPath ) : _closed( true ), _path( dirPath )
	{
		_pattern = dirPath + "/*";
		_readFlag = 0;
		_fileId = 0L;
	}

	~DirIterator()
	{
		closeDir();
	}

	// opens the dir (can also be used to check whether the directory exists)
	bool openDir()
	{
		_closed = ( ( _fileId = _findfirst( _pattern.data(), &_fileHandle ) ) == -1 );
		return !_closed;
	}

	// closes the dir (called by the destructor if needed).
	void closeDir()
	{
		if( !_closed )
			_findclose( _fileId );
	}

	// access the next entry of the directory.
	bool next()
	{
		if( _fileId == -1 )
			return false;

		_readFlag = _findnext( _fileId, &_fileHandle );

		if( _readFlag == -1  )
			return false;

		return true;
	}

	// get the current entry name.
	const char* getName()
	{
		return _fileHandle.name;
	}

private:
	bool _closed;
	std::string _path;

	int _readFlag;
	long _fileId;
	std::string _pattern;
	struct _finddata_t _fileHandle;
};

#else // POSIX

class DirIterator
{
public:
	DirIterator( const std::string& dirPath ) : _closed( true ), _path( dirPath )
	{;}

	~DirIterator()
	{
		closeDir();
	}

	bool openDir()
	{
		_closed = ( _dir = opendir( _path.data() ) ) == NULL;
		return !_closed;
	}

	void closeDir()
	{
		if( !_closed )
			closedir( _dir );
	}

	bool next()
	{
		return ( _entry = readdir( _dir ) ) != NULL;
	}

	const char* getName()
	{
		return _entry->d_name;
	}

private:
	bool _closed;
	std::string _path;

	DIR* _dir;
	struct dirent* _entry;
};

#endif

// utility function, do not normalize paths
// and checks whether a directory exists
// if a file with that name exists it returns false
static bool dirExists( const char* path )
{
	CORAL_STAT_STRUCT info;
	if( CORAL_STAT_FUNC( path, &info ) )
		return false;

	return S_ISDIR( info.st_mode ) != 0;
}

static bool isRootPath( const std::string& path )
{
#ifdef CORAL_OS_WIN
	if( path.length() <= 3 && path.length() > 1 && path.at( 1 ) == ':' )
		return true;
	return false;
#else
	if( path == "/" )
		return true;
	return false;
#endif
}

// convert every '\\' to '/' and remove multiple consecutive "\\"
static void normalizeSeparators( std::string& s )
{
	// search for repeated slashes
	int size = s.length();
	bool found = false;

	// this method is O(n), each while is complementary
	int i = 0;
	while( i < size )
	{
		if( s[i] == '\\' )
		{
			// mark that a windows sep was found, so a replace is needed.
			found = true;
			++i;
			while( s[i] == '\\' && i < size )
			{
				s.erase( i, 1 );
				--size;
			}
		}
		++i;
	}
	if( found )
	{

		for( unsigned int i = 0; i < s.length(); ++i )
		{
			// replaces namespace separator with the folder separator
			if( s[i] == '\\' )
				s[i] = '/';
		}
	}

	if( s[s.length() - 1] == '/' && s.length() > 1 )
	{
#ifdef CORAL_OS_WIN
		if( !( s.length() == 3 && s.at( 1 ) == ':' ) )
#endif
		s.resize( s.length() - 1 );
	}
}

/*
	Utility function: Normalize the path in list.

	Example:
	{ "C:", "temp", "..", "dir" } is converted to { "C:", "dir" }
	{ "C:", "temp", ".", "dir" } is converted to { "C:", "temp", "dir" }
	{ "..", "..", "temp" } is kept untouched because it is a relative path
	{ "..", "..", "temp", ".." } is converted to { "..", ".." }
	{ "..", "..", temp", "..", ".." } is converted to { "..", "..", ".." }
 */
static void normalizePath( std::vector<std::string>& list )
{
	unsigned int i = 0;
	while( i < list.size() )
	{
		if( list[i] == "." )
		{
			list.erase( list.begin() + i );
		}
		else if( list[i] == ".." && i > 0 )
		{
			std::string previous = list[ i -1 ];
#ifdef CORAL_OS_WIN
			if( isRootPath( previous ) )
			{
				// something is wrong, the path is invalid..
				// cannot back further than the root path
				list.erase( list.begin() + i );
				return;
			}
#endif
			if( previous != ".." )
			{
				--i;
				list.erase( list.begin() + i );
				list.erase( list.begin() + i );
			}
			else
			{
				++i;
			}
		}
		else
		{
			// Only increment when no element was removed
			++i;
		}
	}
}

static std::string clean( const std::string& path, bool normalizeSeps = true )
{
	std::string result = path;

	if( normalizeSeps )
		normalizeSeparators( result );

	std::vector<std::string> pathList;
	StringTokenizer st( result, "/" );
	while( st.nextToken() )
		pathList.push_back( st.getToken() );

	normalizePath( pathList );

	std::stringstream finalStream;
	for( std::vector<std::string>::iterator it = pathList.begin(); it != pathList.end(); ++it )
	{
		finalStream << (*it);
		if( ( it + 1 ) != pathList.end() )
			finalStream << '/';
	}

	std::string finalString = finalStream.str();

#ifndef CORAL_OS_WIN
	// restore the last '/', in the case of an absolute path in unix
	if( result.at( 0 ) == '/' )
		finalString.insert( finalString.begin(), '/' );
#else
	if( isRootPath( finalString ) )
	{
		finalString = finalString[0];
		finalString.append( ":\\" );
	}
#endif

	return finalString;
}

std::string Dir::cleanPath( const std::string& path )
{
	if( path.empty() )
		return std::string();

	return clean( path );
}

bool Dir::isAbsolutePath( const std::string& path )
{
	if( path.empty() )
		return false;

#ifndef CORAL_OS_UNIX
	if( path.length() >= 2 && path.at( 1 ) == ':' )
		return true;
	return false;
#else
	if( path.at( 0 ) == '/' )
		return true;
	return false;
#endif
}

bool Dir::makeDir( const std::string& dirPath )
{
	if( dirPath.empty() )
		return false;

	const char *path = dirPath.data();

	int fail;
#ifndef CORAL_OS_UNIX

	int oldmask, err;

	err = _umask_s( 0, &oldmask );

	if( err )
		return false;

	fail = _mkdir( path );

	if( fail )
	{
		_umask_s( oldmask, &err );
		return false;
	}

	_umask_s( oldmask, &err );
#else
	mode_t oldmask = umask( ( mode_t )0 );
	fail =  mkdir( path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP |
						 S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH );
	if( fail )
		return false;

	umask( oldmask );
#endif

	return true;
}

bool Dir::removeDir( const std::string& dirPath )
{
	const char *path = dirPath.data();
#ifdef CORAL_OS_WIN
	return _rmdir( path ) == 0;
#else
	return rmdir( path ) == 0;
#endif
}

bool Dir::makePath( const std::string& dirPath )
{
	if( dirPath.empty() )
		return false;

	std::string path = cleanPath( dirPath );

	// the position to the current '/'
	int currentPos = 0;
#ifdef CORAL_OS_UNIX
	// absolute path contains a '/' at first pos, under unix.
	if( isAbsolutePath( path ) )
		currentPos = 1;
#endif

	// force a '/' at end (cleanPath removes it)
	path += '/';

	// O(n), search for each '/' that delimits subpaths
	while( ( currentPos = path.find( '/', currentPos ) ) != -1 )
	{
		std::string subPath( path.data(), currentPos );
		// do not stop if some dir already exists.. keep creating
		if( !dirExists( subPath.data() ) )
		{

#ifdef CORAL_OS_WIN
			if( ( subPath.length() == 2 ) && ( subPath.find( ':', 1 ) ) )
			{
				subPath.append( "/" );

				if( dirExists( subPath.data() ) )
				{
					++currentPos;
					continue;
				}
			}
#endif

			// stop only if some error occurs while creating the dir
			if( !makeDir( subPath ) )
				return false;
		}
		++currentPos;
	}

	return true;
}

Dir::Dir( const std::string& path )
{
	if( path.empty() )
	{
		_path = ".";
		return;
	}
	_path = cleanPath( path );
}

Dir::~Dir()
{
	// empty
}

const std::string& Dir::getPath() const
{
	return _path;
}

bool Dir::exists() const
{
	return dirExists( _path.c_str() );
}

bool Dir::isFile( const std::string& fileName ) const
{
	CORAL_STAT_STRUCT info;
	if( CORAL_STAT_FUNC( fileName.c_str(), &info ) )
		return false;

	return S_ISREG( info.st_mode ) != 0;
}

bool Dir::isRoot() const
{
	return isRootPath( _path );
}

void Dir::getEntryList( std::vector<std::string>& result )
{
	DirIterator it( _path );

	// cleans string vector
	result.clear();

	if( !it.openDir() )
		return;

	while( it.next() )
	{
		std::string filename( it.getName() );
		// do not add ".." nor "." to the list
		if( filename != ".." && filename != "." && isFile( _path + '/' + filename ) )
			result.push_back( filename );
	}
}

} // namespace co
