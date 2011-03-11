/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_COMPONENTBASE_H_
#define _CO_RESERVED_COMPONENTBASE_H_

#include <co/Component.h>
#include <co/IllegalArgumentException.h>

namespace co {

class CORAL_EXPORT ComponentBase : public Component
{
public:
	ComponentBase() : _refCount( 0 )
	{;}

	virtual ~ComponentBase();

	// co::Interface methods for the 'component' (co::Component) interface:
	InterfaceType* getInterfaceType();
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

	//! Raises co::NoSuchInterfaceException if \a itfInfo isn't one of the component's interfaces.
	void checkValidInterface( InterfaceInfo* itfInfo );

	//! Similar to checkValidInterface(), but also raises an exception if \a receptacle is a facet.
	void checkValidReceptacle( InterfaceInfo* receptacle );

	/*!
		Raises a co::NoSuchInterfaceException for cases that simply "should never happen"
		(e.g. if an interface index is invalid).
	 */
	void raiseUnexpectedInterfaceIndex();

	//! Downcasts an interface pointer, throwing co::IllegalArgumentException if the cast fails.
	template<typename T>
	T* checkedInterfaceCast( Interface* ptr )
	{
		if( !ptr ) return NULL; // null pointers are always compatible
		T* castedPtr = dynamic_cast<T*>( ptr );
		if( !castedPtr )
			raiseIncompatibleInterface( typeOf<T>::get(), ptr );
		return castedPtr;
	}

	//! Raises an IllegalArgumentException stating the passed 'ptr' was not of the 'expectedType'.
	void raiseIncompatibleInterface( InterfaceType* expectedType, Interface* ptr );

	/*!
		Utility method to dynamically define simple, internal component types (i.e.
		components that provide a single interface and don't have reflectors). Used
		internally by the core for all Reflectors and default ModulePart components.

		\param componentTypeName fully-qualified name of the ComponentType.
		\param interfaceTypeName type name of the component's sole facet.
		\param interfaceName interface name of the component's sole facet.
	 */
	ComponentType* getOrCreateSimpleInternalComponentType( const char* componentTypeName,
														   const char* interfaceTypeName,
														   const char* interfaceName );

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
	co::Component* __##ComponentName##_newInstance() { return new ClassName; }

/*!
	Macro that should be called once for each module's main co::ModulePart class.
	\note Both the class and the macro call should be within the module's namespace.
 */
#define CORAL_EXPORT_MODULE_PART( ClassName ) \
	co::ModulePart* createModulePart() { return new ClassName; }

#endif // _CO_RESERVED_COMPONENTBASE_H_
