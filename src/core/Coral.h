/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice at the end of this file.
 */

#ifndef _CO_CORAL_H_
#define _CO_CORAL_H_

#include <co/Platform.h>
#include <co/ArrayRange.h>

namespace co {

// forward declarations:
class Type;
class System;
class Component;
class Interface;
class InterfaceType;

//! Returns the list of type repositories in use by the framework.
CORAL_EXPORT ArrayRange<const std::string> getPaths();

/*!
	Adds one or more type repositories to be used by the framework.
	The passed string may contain a single directory or a list of directories,
	separated by comma or semicolon (or colon, on UNIX systems).
 */
CORAL_EXPORT void addPath( const std::string& path );

/*!
	Returns the co::System bootstrap interface (a singleton).
	This first call to this function (ever, or since the last call to co::shutdown())
	initializes the Coral framework.
 */
CORAL_EXPORT System* getSystem();

/*!
	Tears down the system (if it is running) and destroys it, releasing all memory and resources.
 */
CORAL_EXPORT void shutdown();

/*!
	Utility function to retrieve or load a type by name.
	This is equivalent to calling <tt>co::getSystem()->getTypes()->getType( fullName )</tt>.
	Please refer to co::TypeManager::getType() for the list of exceptions this function may throw.
 */
CORAL_EXPORT Type* getType( const std::string& fullName );

/*!
	Utility function to instantiate a component given its full type name.
	This is equivalent to calling <tt>co::getType( fullName )->getReflector()->newInstance()</tt>.
	Notice that this function may raise all exceptions raised by the aforementioned methods.
 */
CORAL_EXPORT Component* newInstance( const std::string& fullName );

/*!
	Utility function to get the best provider of \c serviceType for clients of type \c clientType.
	If \c clientType is null this function will retrieve the service's global instance.
	Please refer to co::ServiceManager::getServiceForType() for the list of exceptions this function may throw.
 */
CORAL_EXPORT Interface* getServiceForType( InterfaceType* serviceType, InterfaceType* clientType );

/*!
	Utility function to get the best provider of \c serviceType for the given \c clientInstance.
	This is equivalent to calling <tt>co::getSystem()->getServices()->getServiceForInstance()</tt>.
	Please refer to co::ServiceManager::getServiceForInstance() for the list of exceptions this function may throw.
 */
CORAL_EXPORT Interface* getServiceForInstance( InterfaceType* serviceType, Interface* clientInstance );

/*!
	Template function to get a global service by its interface type.
	Please refer to co::ServiceManager::getService() for the list of exceptions this function may throw.
 */
template<typename T>
inline T* getService()
{
	return static_cast<T*>( getServiceForType( co::typeOf<T>::get(), NULL ) );
}

/*!
	Template function to get a \c clientType-specialized service by its interface type.
	This picks the most appropriate service instance available for clients of the given \c clientType.
	Please refer to co::ServiceManager::getServiceForType() for the list of exceptions this function may throw.
 */
template<typename T>
inline T* getService( co::InterfaceType* clientType )
{
	return static_cast<T*>( getServiceForType( co::typeOf<T>::get(), clientType ) );
}

/*!
	Template function to get a \c clientInstance-specialized service by its interface type.
	This picks the most appropriate service instance available for the given \c clientInstance.
	Please refer to co::ServiceManager::getServiceForInstance() for the list of exceptions this function may throw.
 */
template<typename T>
inline T* getService( co::Interface* clientInstance )
{
	return static_cast<T*>( getServiceForInstance( co::typeOf<T>::get(), clientInstance ) );
}

} // namespace co

/******************************************************************************
 
 Copyright (c) 2009-2010 Tecgraf, PUC-Rio.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 *****************************************************************************/

#endif
