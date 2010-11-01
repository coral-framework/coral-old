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

//---------- Coral Path -----------------------------------------------------//

/*!
	\brief Returns the list of type repositories in use by the system.
	\ingroup setup
 */
CORAL_EXPORT ArrayRange<const std::string> getPaths();

/*!
	\brief Adds one or more type repositories for use by the system.
 
	The passed string may contain a single directory or a list of directories,
	separated by comma or semicolon (or colon, on UNIX systems).

	\ingroup setup
 */
CORAL_EXPORT void addPath( const std::string& path );


//---------- Bootstrap and Shutdown -----------------------------------------//

/*!
	\brief Returns the co.System bootstrap interface (a singleton).

	This first call to this function (ever, or since the last call to co::shutdown())
	initializes the Coral framework.

	\ingroup setup
 */
CORAL_EXPORT System* getSystem();

/*!
	\brief Tears down and destroys the system, releasing all memory and resources.
	\ingroup setup
 */
CORAL_EXPORT void shutdown();


//---------- Debug Events ---------------------------------------------------//

/*!
	The kind of events that can be sent to a DebugEventHandler.
	\ingroup debugevents
 */
enum DebugEvent
{
	Dbg_Message,	//!< A simple debug message. Not sent in Release builds.
	Dbg_Warning,	//!< A Warning message. Something worth checking.
	Dbg_Critical,	//!< A Critical error. Potential data loss and malfunction.
	Dbg_Fatal		//!< Fatal error. Default action is to abort the application.
};

/*!
	Signature of a DebugEventHandler function. See installDebugEventHandler().
	\ingroup debugevents
 */
typedef void (*DebugEventHandler)( DebugEvent event, const char* message );

/*!
	Installs a debug event handler and returns the previous handler (which may be NULL).

	A debug event handler is a function that prints out debug messages, warnings, critical and fatal
	error messages. The default handler prints all messages to \c stderr, and aborts the application
	in case of a fatal error. Only a single debug event handler can be defined per application, and
	it should generally be installed before the framework is initialized.
	To restore the default handler, call <tt>installDebugEventHandler( NULL )</tt>.

	 \sa debug()
	 \ingroup debugevents
 */
CORAL_EXPORT DebugEventHandler installDebugEventHandler( DebugEventHandler handler );

/*!
	Sends a debug event to the currently-installed debug event handler.

	The debug event API is used as an alternative to exception handling
	when it is impossible or unnecessary to raise an exception.

	The first parameter specifies the kind of the event (see DebugEvent).
	The remaining parameters are a format string and an optional list of
	arguments that will compose the debug message, just like in printf().

	 \sa installDebugEventHandler()
	 \ingroup debugevents
 */
CORAL_EXPORT void debug( DebugEvent event, const char* msg, ... );


//---------- General Utility Functions --------------------------------------//

/*!
	Utility function to retrieve or load a type by name.
	This is equivalent to calling <tt>co::getSystem()->getTypes()->getType( fullName )</tt>.
	Please refer to co::TypeManager::getType() for the list of exceptions this function may throw.
	\ingroup convenience
 */
CORAL_EXPORT Type* getType( const std::string& fullName );

/*!
	Utility function to instantiate a component given its full type name.
	This is equivalent to calling <tt>co::getType( fullName )->getReflector()->newInstance()</tt>.
	Notice that this function may raise all exceptions raised by the aforementioned methods.
	\ingroup convenience
 */
CORAL_EXPORT Component* newInstance( const std::string& fullName );

/*!
	Utility function to get the best provider of \c serviceType for clients of type \c clientType.
	If \c clientType is null this function will retrieve the service's global instance.
	Please refer to co::ServiceManager::getServiceForType() for the list of exceptions this function may throw.
	\ingroup convenience
 */
CORAL_EXPORT Interface* getServiceForType( InterfaceType* serviceType, InterfaceType* clientType );

/*!
	Utility function to get the best provider of \c serviceType for the given \c clientInstance.
	This is equivalent to calling <tt>co::getSystem()->getServices()->getServiceForInstance()</tt>.
	Please refer to co::ServiceManager::getServiceForInstance() for the list of exceptions this function may throw.
	\ingroup convenience
 */
CORAL_EXPORT Interface* getServiceForInstance( InterfaceType* serviceType, Interface* clientInstance );

/*!
	Template function to get a global service by its interface type.
	Please refer to co::ServiceManager::getService() for the list of exceptions this function may throw.
	\ingroup convenience
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
	\ingroup convenience
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
	\ingroup convenience
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
