/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_COMPONENTBASE_H_
#define _CO_RESERVED_COMPONENTBASE_H_

#include <co/IObject.h>
#include <co/IllegalArgumentException.h>

namespace co {

/*!
	\brief Base class for all components.
	Provides the implicit 'object' (co::IObject) facet.
 */
class CORAL_EXPORT ComponentBase : public IObject
{
public:
	ComponentBase() : _refCount( 0 )
	{;}

	virtual ~ComponentBase();

	// co::IService methods for the 'object' (co::IObject) facet
	IInterface* getInterfaceType();
	const std::string& getInterfaceName();

protected:
	inline void incrementRefCount()
	{
		++_refCount;
	}

	inline void decrementRefCount()
	{
		if( --_refCount <= 0 )
			delete this;
	}

	//! Raises co::NoSuchPortException if \a itfInfo isn't one of the component's interfaces.
	void checkValidPort( IPort* port );

	//! Similar to checkValidPort(), but also raises an exception if \a receptacle is a facet.
	void checkValidReceptacle( IPort* receptacle );

	//! Raises a NoSuchPortException for cases that "should never happen" (e.g. invalid port index).
	void raiseUnexpectedPortIndex();

	//! Downcasts a service, throwing co::IllegalArgumentException if the cast fails.
	template<typename T>
	inline T* checkedCast( IService* ptr )
	{
		if( !ptr ) return NULL; // null pointers are always compatible
		T* castedPtr = dynamic_cast<T*>( ptr );
		if( !castedPtr )
			raiseIncompatibleService( typeOf<T>::get(), ptr );
		return castedPtr;
	}

	//! Raises an IllegalCastException for when a \a service is not of the \a expectedType.
	void raiseIncompatibleService( IInterface* expectedType, IService* service );

	/*!
		Utility method to dynamically define simple, internal component types (i.e.
		components that provide a single interface and don't have reflectors). Used
		internally by the core for all Reflectors and default IModulePart components.

		\param componentName fully-qualified name of the IComponent.
		\param interfaceName type of the component's sole facet.
		\param facetName name of the component's sole facet.
	 */
	IComponent* getOrCreateInternalComponent( const char* componentName,
							const char* interfaceName, const char* facetName );

private:
	int32 _refCount;
};

} // namespace co

/*!
	Macro that should be called once for every component implementation class.
	\note Both the class and the macro call should be within the module's namespace.
 */
#define CORAL_EXPORT_COMPONENT( ClassName, ComponentName ) \
	co::int32 __##ComponentName##_getSize() { return sizeof(ClassName); } \
	co::IObject* __##ComponentName##_newInstance() { return new ClassName; }

/*!
	Macro that should be called once for each module's main co::IModulePart class.
	\note Both the class and the macro call should be within the module's namespace.
 */
#define CORAL_EXPORT_MODULE_PART( ClassName ) \
	co::IModulePart* createModulePart() { return new ClassName; }

#endif // _CO_RESERVED_COMPONENTBASE_H_
