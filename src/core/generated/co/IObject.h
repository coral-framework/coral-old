/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IOBJECT_H_
#define _CO_IOBJECT_H_

#include <co/TypeTraits.h>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IComponent;
	class IPort;
} // namespace co
// End Of Forward Declarations

// co.IObject Mapping:
namespace co {

class IObject : public co::IService
{
public:
	virtual ~IObject() {;}

	// Code From <c++ Block:
	
		/*!
			Retrieves a service provided by this object by its type.
			Result is undefined if the component provides more than one service of type 'T'.
			\tparam T interface of the requested service.
		 */
		template<typename T> inline T* getService()
		{
			return static_cast<T*>( getServiceByType( this, typeOf<T>::get() ) );
		}

		// Returns the service "provided by" or "bound to" the port named \a portName.
		inline IService* getService( const std::string& portName )
		{
			return getServiceByName( this, portName );
		}

		// Binds a \a service to the receptacle named \a receptacleName.
		inline void setService( const std::string& receptacleName, IService* service )
		{
			setServiceByName( this, receptacleName, service );
		}
	
	// End Of c++> Block

	virtual co::IComponent* getComponent() = 0;

	virtual co::IService* getService( co::IPort* port ) = 0;

	virtual void setService( co::IPort* receptacle, co::IService* service ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IObject> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IObject> { static const char* get() { return "co.IObject"; } };
template<> struct typeOf<co::IObject> : public typeOfBase<co::IObject, IInterface> {};
} // namespace co

#endif // _CO_IOBJECT_H_
