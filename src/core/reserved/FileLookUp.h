/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_FILELOOKUP_H_
#define _CO_RESERVED_FILELOOKUP_H_

#include <co/Platform.h>
#include <co/ArrayRange.h>
#include <string>

namespace co {

/*!
	\brief Searches for a file within a list of directories in the filesystem.

	The search supports multiple possible file names (and/or relative locations), specified by the
	\c filePathList property; as well as multiple file extensions, specified by \c extensionList.

	File name combinations are evaluated in the same order they're listed, and in case of ambiguities
	the search will always return the first match.
 */
class CORAL_EXPORT FileLookUp
{
public:
	/*!
		Constructs a FileLookUp instance for locating files within the given \c dirList.
		It only accepts files ending with one of the file extensions listed in \c extensionList.
	 */
	FileLookUp( ArrayRange<const std::string> dirList, ArrayRange<const std::string> extensionList );

	//! Destructor.
	~FileLookUp();

	//! Returns the list of directories iterated in the lookups.
	inline ArrayRange<const std::string> getDirList() const { return _dirList; }

	//! Returns the list of possible names/paths for the file we're looking for.
	inline ArrayRange<const std::string> getFilePathList() const { return _filePathList; }

	//! Returns the list of accepted file extensions.
	inline ArrayRange<const std::string> getExtensionList() const { return _extensionList; }

	/*!
		Adds an alternative name for the file we're looking for.
		The name can be a relative path, and should be extensionless.
		If \c convertSeparators is passed as \c true, any dot ('.') in the \c filePath
		will be converted to a slash ('/').
	 */
	void addFilePath( const std::string& filePath, bool convertSeparators = false );

	//! Clears the list of possible names/paths for the file we're looking for.
	void clearFilePathList();

	/*!
		Iterates all dirs in getDirList(), looking for a file with a name listed in getFilePathLists(),
		having an extension that is listed in getExtensionList().
		
		If a match is found, this method returns true; \c filename is set with the first match,
		and the optional parameters \c dir, \c path and \c ext, when not NULL, are set to the
		corresponding matched sub-parts.
		When no match is found, the method returns false and parameters are not modified.
	  */
	bool locate( std::string& filename, std::string* dir = 0, std::string* path = 0, std::string* ext = 0 );

private:
	//! Returns if the \c filePath is a valid path and if it corresponds to an existing file on the system.
	bool fileExists( const std::string& filePath );

private:
	ArrayRange<const std::string> _dirList;
	ArrayRange<const std::string> _extensionList;
	std::vector<std::string> _filePathList;
};

} // namespace co

#endif // _CO_RESERVED_FILELOOKUP_H_
