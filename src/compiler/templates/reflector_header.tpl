/*******************************************************************************
** Reflection code generated for type '{{TYPE_NAME_FULL}}'
**
** Created: {{CURRENT_DATE_TIME}}
**      by: Coral Compiler version {{CORAL_VERSION}}
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

{{NEWLINE}}

{{#IS_COMPONENT_FALSE}}
{{#IS_NATIVE_TRUE}}
#include <{{TYPE_NAME}}_Adapter.h>
{{/IS_NATIVE_TRUE}}
{{#IS_NATIVE_FALSE}}
#include <{{TYPE_FILE_PATH}}.h>
{{/IS_NATIVE_FALSE}}
{{/IS_COMPONENT_FALSE}}
#include <co/reserved/ReflectorBase.h>
{{#IS_INTERFACE_TRUE}}
#include <co/DynamicProxyHandler.h>
{{/IS_INTERFACE_TRUE}}
{{#FW_DECL_LIST}}
#include <{{HEADER}}>
{{/FW_DECL_LIST}}
{{#IS_COMPOUND_TYPE}}
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
{{#IS_METHOD_CONTAINER}}
#include <co/IllegalCastException.h>
{{/IS_METHOD_CONTAINER}}
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
{{/IS_COMPOUND_TYPE}}
{{#IS_REFLECTOR_REFLECTOR}}
#include <co/UnsupportedOperationException.h>
{{/IS_REFLECTOR_REFLECTOR}}
{{#IS_COMPOUND_TYPE}}
#include <sstream>
{{/IS_COMPOUND_TYPE}}
#include <cassert>

{{NEWLINE}}

{{#MODULE_IS_CO_FALSE}}
#if !defined( CORAL_COMPILER_OUTPUT_REVISION )
#error "The header file <{{TYPE_FILE_PATH}}.h> doesn't include <co/Config.h>."
#elif CORAL_COMPILER_OUTPUT_REVISION != {{OUTPUT_REVISION}}
#error "This file was generated using the Coral Compiler v{{CORAL_VERSION}}. It"
#error "cannot be used with the include files from this version of Coral."
#error "(The code generation rules have changed too much.)"
#endif

{{NEWLINE}}
{{/MODULE_IS_CO_FALSE}}

{{#IS_COMPONENT_TRUE}}
// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __{{TYPE_NAME}}_getSize();
co::Component* __{{TYPE_NAME}}_newInstance();

{{NEWLINE}}
{{/IS_COMPONENT_TRUE}}

{{#NAMESPACE_LIST}}
namespace {{NAMESPACE}} {
{{/NAMESPACE_LIST}}

{{#MODULE_IS_CO_FALSE}}
{{NEWLINE}}
void moduleRetain();
void moduleRelease();
{{/MODULE_IS_CO_FALSE}}

{{NEWLINE}}

{{#IS_INTERFACE_TRUE}}
// ------ Proxy Interface ------ //

{{NEWLINE}}

class {{TYPE_NAME}}_Proxy : public {{TYPE_NAME_CPP}}
{
public:
	{{TYPE_NAME}}_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
{{#MODULE_IS_CO_FALSE}}
		moduleRetain();
{{/MODULE_IS_CO_FALSE}}
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, {{TYPE_NAME_CPP}}>( this ) );
	}

{{NEWLINE}}

	virtual ~{{TYPE_NAME}}_Proxy()
	{
{{#MODULE_IS_CO_FALSE}}
		moduleRelease();
{{/MODULE_IS_CO_FALSE}}
{{#MODULE_IS_CO_TRUE}}
		// empty
{{/MODULE_IS_CO_TRUE}}
	}

{{NEWLINE}}

	// co::Interface Methods:
{{NEWLINE}}
	co::InterfaceType* getInterfaceType() { return co::typeOf<{{TYPE_NAME_CPP}}>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

{{NEWLINE}}

{{#ANCESTOR_LIST}}
	// {{ANCESTOR_CLASS}} Methods:

{{NEWLINE}}

	{{#ATTRIBUTE_LIST}}
		{{#GETTER}}
	{{RETURN_TYPE}} {{METHOD_NAME}}()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<{{ANCESTOR_CLASS}}>( {{ATTRIBUTE_INDEX}} ), __res );
        return __res.get< {{RETURN_TYPE}} >();
	}{{BI_NEWLINE}}
		{{/GETTER}}
		{{#SETTER}}
	void {{METHOD_NAME}}( {{PARAMETER_TYPE}} {{PARAMETER_NAME}} )
	{
		co::Any __arg;
		__arg.set< {{PARAMETER_TYPE}} >( {{PARAMETER_NAME}} );
		_handler->handleSetAttribute( _cookie, getAttribInfo<{{ANCESTOR_CLASS}}>( {{ATTRIBUTE_INDEX}} ), __arg );
	}{{BI_NEWLINE}}
		{{/SETTER}}
	{{/ATTRIBUTE_LIST}}

	{{#METHOD_LIST}}
	{{RETURN_TYPE}} {{METHOD_NAME}}({{#PARAMETER_LIST}} {{PARAMETER_TYPE}} {{PARAMETER_NAME}}{{#PARAMETER_LIST_separator}},{{/PARAMETER_LIST_separator}}{{/PARAMETER_LIST}} )
	{
	{{#HAS_PARAMETERS_TRUE}}
		co::Any __res, __arg[{{PARAMETER_COUNT}}];
		{{#PARAMETER_LIST}}
		__arg[{{PARAMETER_INDEX}}].set< {{PARAMETER_TYPE}} >( {{PARAMETER_NAME}} );
		{{/PARAMETER_LIST}}
		co::ArrayRange<co::Any const> __ar( __arg, {{PARAMETER_COUNT}} );
	{{/HAS_PARAMETERS_TRUE}}
	{{#HAS_PARAMETERS_FALSE}}
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
	{{/HAS_PARAMETERS_FALSE}}
		_handler->handleMethodInvocation( _cookie, getMethodInfo<{{ANCESTOR_CLASS}}>( {{METHOD_INDEX}} ), __ar, __res );
	{{#HAS_RETURN}}
		return __res.get< {{RETURN_TYPE}} >();
	{{/HAS_RETURN}}
	}{{BI_NEWLINE}}
	{{/METHOD_LIST}}
{{/ANCESTOR_LIST}}

{{#IS_REFLECTOR_REFLECTOR}}
	// These co::Reflector methods are not part of the reflection system:
{{NEWLINE}}
	void createValue( void*, size_t )
	{
		throw co::UnsupportedOperationException( "co::Reflector::createValue() cannot be called through a proxy interface." );
	}
{{NEWLINE}}
	void destroyValue( void* )
	{
		throw co::UnsupportedOperationException( "co::Reflector::destroyValue() cannot be called through a proxy interface." );
	}
{{NEWLINE}}
{{/IS_REFLECTOR_REFLECTOR}}

protected:
	template<typename T>
	co::AttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

{{NEWLINE}}

	template<typename T>
	co::MethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

{{NEWLINE}}

private:
	co::DynamicProxyHandler* _handler;
	co::uint32 _cookie;
};
{{NEWLINE}}
{{/IS_INTERFACE_TRUE}}

// ------ Reflector ------ //

{{NEWLINE}}

class {{TYPE_NAME}}_Reflector : public co::ReflectorBase
{
public:
	{{TYPE_NAME}}_Reflector()
	{
{{#MODULE_IS_CO_FALSE}}
		moduleRetain();
{{/MODULE_IS_CO_FALSE}}
{{#MODULE_IS_CO_TRUE}}
		// empty
{{/MODULE_IS_CO_TRUE}}
	}

{{NEWLINE}}

	virtual ~{{TYPE_NAME}}_Reflector()
	{
{{#MODULE_IS_CO_FALSE}}
		moduleRelease();
{{/MODULE_IS_CO_FALSE}}
{{#MODULE_IS_CO_TRUE}}
		// empty
{{/MODULE_IS_CO_TRUE}}
	}

{{NEWLINE}}

	co::Type* getType()
	{
{{#IS_COMPONENT_FALSE}}
		return co::typeOf<{{TYPE_NAME_CPP}}>::get();
{{/IS_COMPONENT_FALSE}}
{{#IS_COMPONENT_TRUE}}
		return co::getType( "{{TYPE_NAME_FULL}}" );
{{/IS_COMPONENT_TRUE}}
	}

{{#IS_COMPONENT_FALSE}}
{{NEWLINE}}
	co::int32 getSize()
	{
		return sizeof({{TYPE_NAME_CPP}});
	}
{{/IS_COMPONENT_FALSE}}

{{#IS_COMPONENT_TRUE}}
{{NEWLINE}}
	co::int32 getSize()
	{
		return __{{TYPE_NAME}}_getSize();
	}
{{NEWLINE}}
	co::Component* newInstance()
	{
		co::Component* component = __{{TYPE_NAME}}_newInstance();
		assert( component->getComponentType()->getFullName() == "{{TYPE_NAME_FULL}}" );
		return component;
	}
{{/IS_COMPONENT_TRUE}}

{{#IS_COMPLEX_VALUE}}
{{NEWLINE}}
	void createValue( void* address, size_t length )
	{
		checkValidSize( sizeof({{TYPE_NAME_CPP}}), length );
		new( address ) {{TYPE_NAME_CPP}};
    }
{{NEWLINE}}
	void destroyValue( void* address )
	{
		callDestructor( reinterpret_cast<{{TYPE_NAME_CPP}}*>( address ) );
	}
{{/IS_COMPLEX_VALUE}}

{{#IS_INTERFACE_TRUE}}
{{NEWLINE}}
	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, {{TYPE_NAME_CPP}}>( new {{NS}}::{{TYPE_NAME}}_Proxy( handler ) );
	}
{{/IS_INTERFACE_TRUE}}
