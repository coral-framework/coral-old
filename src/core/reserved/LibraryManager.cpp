/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "LibraryManager.h"
#include "../tools/Library.h"
#include <map>
#include <vector>

typedef std::map<co::ModulePart*, co::RefPtr<co::Library> > LibraryMap;
typedef std::vector<co::RefPtr<co::Library> > LibraryList;

namespace
{
	LibraryMap sg_loadedLibs;
	LibraryList sg_releasedLibs;
}

void co::LibraryManager::add( co::ModulePart* part, co::Library* library )
{	
	sg_loadedLibs.insert( LibraryMap::value_type( part, library ) );
}

void co::LibraryManager::release( co::ModulePart* part )
{
	LibraryMap::iterator it = sg_loadedLibs.find( part );
	if( it == sg_loadedLibs.end() )
	{
		assert( false );
		return;
	}

	// unload the library
	sg_releasedLibs.push_back( it->second );
	sg_loadedLibs.erase( it );
}

void co::LibraryManager::releaseAll()
{
	for( LibraryMap::iterator it = sg_loadedLibs.begin(); it != sg_loadedLibs.end(); ++it )
		sg_releasedLibs.push_back( it->second );
	sg_loadedLibs.clear();
}

void co::LibraryManager::flush()
{
	sg_releasedLibs.clear();
}
