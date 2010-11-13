/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "FileLookUp.h"
#include "../tools/FileSystem.h"
#include <sys/stat.h>
#include <sstream>

co::FileLookUp::FileLookUp( co::ArrayRange<const std::string> dirList, co::ArrayRange<const std::string> extensionList )
	: _dirList( dirList ), _extensionList( extensionList )
{
	// empty
}

co::FileLookUp::~FileLookUp()
{
	// empty
}

void co::FileLookUp::addFilePath( const std::string& filePath, bool convertSeparators )
{
	if( !convertSeparators )
	{
		_filePathList.push_back( filePath );
		return;
	}

	std::string path( filePath );
	for( unsigned int i = 0; i < path.length(); ++i )
	{
		// replaces namespace separator with the folder separator
		if( path[i] == '.' )
			path[i] = '/';
	}

	_filePathList.push_back( path );

}

void co::FileLookUp::clearFilePathList()
{
	_filePathList.clear();
}

bool co::FileLookUp::locate( std::string& filename, std::string* dir, std::string* path, std::string* ext )
{
	std::string filePath;
	std::stringstream strStream;

	for( co::ArrayRange<const std::string> dirs( _dirList ); dirs; dirs.popFirst() )
	{
		for( std::vector<std::string>::const_iterator pathIt = _filePathList.begin(); pathIt != _filePathList.end(); ++pathIt )
		{
			for( co::ArrayRange<const std::string> extensions( _extensionList ); extensions; extensions.popFirst() )
			{
				strStream.str( "" );

				strStream << dirs.getFirst() << '/' << *pathIt;

				const std::string& extension = extensions.getFirst();
				if( !extension.empty() )
				{
					strStream << '.' << extension;
				}

				filePath = strStream.str();
				if( fileExists( filePath ) )
				{
					filename = filePath;

					if( dir ) *dir = dirs.getFirst();
					if( path ) *path =  *pathIt;
					if( ext ) *ext = extensions.getFirst();

					return true;
				}
			}
		}
	}

	return false;
}

bool co::FileLookUp::fileExists( const std::string& filePath )
{
	CORAL_STAT_STRUCT info;
	if( CORAL_STAT_FUNC( filePath.c_str(), &info ) )
		return false;
	return S_ISREG( info.st_mode ) != 0;
}
