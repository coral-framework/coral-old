/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_LIBRARYMANAGER_H_
#define _CO_RESERVED_LIBRARYMANAGER_H_

#include <co/Platform.h>

namespace co {

class Library;
class ModulePart;

/*!
	Auxiliary static class that keeps track of all co::Library instances
	(corresponding to the loaded module's shared libs) until they can be unloaded.
 */
class CORAL_EXPORT LibraryManager
{
public:
	//! Registers a module part and its associated library.
	static void add( ModulePart* part, Library* library );

	/*!
		Releases the library associated with a module part,
		so it can be unloaded by the next flush().
	 */
	static void release( ModulePart* part );

	//! Releases all registered libraries.
	static void releaseAll();

	//! Unloads all released libraries that are still loaded.
	static void flush();
};

} // namespace co

#endif // _CO_RESERVED_LIBRARYMANAGER_H_
