/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_DIR_H_
#define _CO_DIR_H_

#include <co/Platform.h>
#include <string>
#include <vector>

//! Main namespace of this library.
namespace co {

/*!
	\brief The Dir class provides access to directory structures and their contents.

	A Dir is used to manipulate path names, access information regarding paths and files,
	and manipulate the underlying file system. This class uses "/" as a universal directory
	separator in the same way that "/" is used as a path separator in URLs. If a path is
	given with "\" separator, it will be automatically converted to a path containing
	only "/" separators. A Dir can point to a file using either a relative or an absolute
	path. Absolute paths begin with the directory separator (optionally preceded by a drive
	specification under Windows). Relative file names begin with a directory name or a file
	name and specify a path relative to the current directory.
 */
class CORAL_EXPORT Dir
{
public:
	/*!
		Returns true if path is absolute; returns false if it is relative.
		\see isAbsolute(), isRelativePath(), makeAbsolute(), and cleanPath().
	 */
	static bool isAbsolutePath( const std::string& path );

	/*!
		Creates a directory defined by a specific path. If any directory component in dirPath
		does not exist or is a dangling symbolic link, no new directory is created and the
		function returns false. If the new directory is successfully created, the function returns
		true.
		\see makeSubDir().
	 */
	static bool makeDir( const std::string& dirPath );

	/*!
		Removes the directory specified by dirName. The directory must be empty for removeDir()
		to succeed. Returns true if successful; otherwise returns false.
		\see makeDir(), makeSubDir().
	*/
	static bool removeDir( const std::string& dirPath );

	/*!
		Creates the directory path dirPath. The function will create all parent directories
		necessary to create the directory. Returns true if successful; otherwise returns false.
		If any of the subpaths of the given path exists, it will not be altered and this
		function will try to create all other subpaths. If all subpaths already exists this
		function also returns true.
		\see removePath(), makeDir(), makeSubDir().
	 */
	static bool makePath( const std::string& dirPath );

public:
	//! Constructs a Dir pointing to the given directory path.
	Dir( const std::string& path );

	//! Destroys the Dir object.
	~Dir();

	/*!
		Returns the path. This may contain symbolic links, but never contains
		redundant ".", ".." or multiple separators.

		The returned path can be either absolute or relative. \see setPath().
	 */
	const std::string& getPath() const;


	/*!
		Returns true if the directory exists; otherwise returns false.
		(If a file with the same name is found this function will return false).
	 */
	bool exists() const;

	//! Returns true if \c fileName points to a file. Returns false if the object points
	// to something which isn't a file, such as a directory.
	bool isFile( const std::string& fileName ) const;

	//! Returns true if the directory is the root directory; otherwise returns false.
	bool isRoot() const;

	/*!
		Returns a list of the names of all the files in the directory. Returns an empty list if the
		directory is unreadable, if it does not exist or if the directory is empty.
	 */
	void getEntryList( std::vector<std::string>& result );

private:
	/*!
		Normalizes and cleans the given path. The function converts all
		separators "\" to "/" and removes all multiple directory separators
		"/" and resolves any "."s or ".."s found in the path.
		Symbolic links are kept. This function does not return the canonical
		path, but rather the simplest version of the input.
		For example, "./local" becomes "local", "local/../bin" becomes "bin"
		and "/local/usr/../bin" becomes "/local/bin".
	 */
	static std::string cleanPath( const std::string& path );

private:
	std::string _path;
};

}; // namespace co


#endif
