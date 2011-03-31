/*
 * Coral - A lightweight C++ component framework
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
			<tt>getService<T>()</tt>: convenience method that returns a built-in service of type 'T'
			provided by the component. Please notice that this method only works for static facets.
			It will not retrieve services from dynamic facets, such as those implemented in Lua.
		 */
		template<typename T> inline T* getService() { return dynamic_cast<T*>( this ); }
	
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
