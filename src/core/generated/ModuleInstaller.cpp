/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModuleInstaller.h"
#include <co/Type.h>

co::ModuleInstaller co::ModuleInstaller::sm_instance;

co::ModuleInstaller::ModuleInstaller()
{
	// empty
}

co::ModuleInstaller::~ModuleInstaller()
{
	// empty
}

void co::ModuleInstaller::initialize()
{
	for( unsigned int i = 0; i < TypeCount; ++i )
	{
		if( !_reflectors[i].isValid() )
			_reflectors[i] = createReflector( static_cast<TypeId>( i ) );
	}
}

void co::ModuleInstaller::install()
{
	initialize();

	for( unsigned int i = 0; i < TypeCount; ++i )
	{
		co::Reflector* reflector = _reflectors[i].get();
		reflector->getType()->setReflector( reflector );
	}
}

void co::ModuleInstaller::uninstall()
{
	for( unsigned int i = 0; i < TypeCount; ++i )
	{
		if( _reflectors[i].isValid() )
		{
			_reflectors[i]->getType()->setReflector( NULL );
			_reflectors[i] = NULL;
		}
	}
}

co::Reflector* co::ModuleInstaller::createReflector( TypeId typeId )
{
	co::Reflector* res = NULL;
	switch( typeId )
	{
	case TypeId_ArrayType: res = co::__createArrayTypeReflector(); break;
	case TypeId_ArrayTypeComponent: res = co::__createArrayTypeComponentReflector(); break;
	case TypeId_AttributeContainer: res = co::__createAttributeContainerReflector(); break;
	case TypeId_AttributeInfo: res = co::__createAttributeInfoReflector(); break;
	case TypeId_AttributeInfoComponent: res = co::__createAttributeInfoComponentReflector(); break;
	case TypeId_Component: res = co::__createComponentReflector(); break;
	case TypeId_ComponentType: res = co::__createComponentTypeReflector(); break;
	case TypeId_ComponentTypeComponent: res = co::__createComponentTypeComponentReflector(); break;
	case TypeId_CompoundType: res = co::__createCompoundTypeReflector(); break;
	case TypeId_CSLError: res = co::__createCSLErrorReflector(); break;
	case TypeId_DynamicProxyHandler: res = co::__createDynamicProxyHandlerReflector(); break;
	case TypeId_EnumType: res = co::__createEnumTypeReflector(); break;
	case TypeId_EnumTypeComponent: res = co::__createEnumTypeComponentReflector(); break;
	case TypeId_ExceptionType: res = co::__createExceptionTypeReflector(); break;
	case TypeId_ExceptionTypeComponent: res = co::__createExceptionTypeComponentReflector(); break;
	case TypeId_IllegalArgumentException: res = co::__createIllegalArgumentExceptionReflector(); break;
	case TypeId_IllegalCastException: res = co::__createIllegalCastExceptionReflector(); break;
	case TypeId_IllegalNameException: res = co::__createIllegalNameExceptionReflector(); break;
	case TypeId_Interface: res = co::__createInterfaceReflector(); break;
	case TypeId_InterfaceInfo: res = co::__createInterfaceInfoReflector(); break;
	case TypeId_InterfaceInfoComponent: res = co::__createInterfaceInfoComponentReflector(); break;
	case TypeId_InterfaceType: res = co::__createInterfaceTypeReflector(); break;
	case TypeId_InterfaceTypeComponent: res = co::__createInterfaceTypeComponentReflector(); break;
	case TypeId_LifeCycleException: res = co::__createLifeCycleExceptionReflector(); break;
	case TypeId_MemberInfo: res = co::__createMemberInfoReflector(); break;
	case TypeId_MethodBuilder: res = co::__createMethodBuilderReflector(); break;
	case TypeId_MethodBuilderComponent: res = co::__createMethodBuilderComponentReflector(); break;
	case TypeId_MethodContainer: res = co::__createMethodContainerReflector(); break;
	case TypeId_MethodInfo: res = co::__createMethodInfoReflector(); break;
	case TypeId_MethodInfoComponent: res = co::__createMethodInfoComponentReflector(); break;
	case TypeId_MissingInputException: res = co::__createMissingInputExceptionReflector(); break;
	case TypeId_MissingServiceException: res = co::__createMissingServiceExceptionReflector(); break;
	case TypeId_Module: res = co::__createModuleReflector(); break;
	case TypeId_ModuleComponent: res = co::__createModuleComponentReflector(); break;
	case TypeId_ModuleLoadException: res = co::__createModuleLoadExceptionReflector(); break;
	case TypeId_ModuleManager: res = co::__createModuleManagerReflector(); break;
	case TypeId_ModuleManagerComponent: res = co::__createModuleManagerComponentReflector(); break;
	case TypeId_ModulePart: res = co::__createModulePartReflector(); break;
	case TypeId_ModulePartLoader: res = co::__createModulePartLoaderReflector(); break;
	case TypeId_ModulePartLoaderComponent: res = co::__createModulePartLoaderComponentReflector(); break;
	case TypeId_ModuleState: res = co::__createModuleStateReflector(); break;
	case TypeId_Namespace: res = co::__createNamespaceReflector(); break;
	case TypeId_NamespaceComponent: res = co::__createNamespaceComponentReflector(); break;
	case TypeId_NativeClassType: res = co::__createNativeClassTypeReflector(); break;
	case TypeId_NativeClassTypeComponent: res = co::__createNativeClassTypeComponentReflector(); break;
	case TypeId_NoSuchInterfaceException: res = co::__createNoSuchInterfaceExceptionReflector(); break;
	case TypeId_ParameterInfo: res = co::__createParameterInfoReflector(); break;
	case TypeId_ParameterInfoComponent: res = co::__createParameterInfoComponentReflector(); break;
	case TypeId_Reflector: res = co::__createReflectorReflector(); break;
	case TypeId_SemanticException: res = co::__createSemanticExceptionReflector(); break;
	case TypeId_ServiceManager: res = co::__createServiceManagerReflector(); break;
	case TypeId_ServiceManagerComponent: res = co::__createServiceManagerComponentReflector(); break;
	case TypeId_StructType: res = co::__createStructTypeReflector(); break;
	case TypeId_StructTypeComponent: res = co::__createStructTypeComponentReflector(); break;
	case TypeId_System: res = co::__createSystemReflector(); break;
	case TypeId_SystemComponent: res = co::__createSystemComponentReflector(); break;
	case TypeId_SystemState: res = co::__createSystemStateReflector(); break;
	case TypeId_Type: res = co::__createTypeReflector(); break;
	case TypeId_TypeBuilder: res = co::__createTypeBuilderReflector(); break;
	case TypeId_TypeBuilderComponent: res = co::__createTypeBuilderComponentReflector(); break;
	case TypeId_TypeComponent: res = co::__createTypeComponentReflector(); break;
	case TypeId_TypeCreationTransaction: res = co::__createTypeCreationTransactionReflector(); break;
	case TypeId_TypeCreationTransactionComponent: res = co::__createTypeCreationTransactionComponentReflector(); break;
	case TypeId_TypeKind: res = co::__createTypeKindReflector(); break;
	case TypeId_TypeLoadException: res = co::__createTypeLoadExceptionReflector(); break;
	case TypeId_TypeManager: res = co::__createTypeManagerReflector(); break;
	case TypeId_TypeManagerComponent: res = co::__createTypeManagerComponentReflector(); break;
	case TypeId_NotSupportedException: res = co::__createNotSupportedExceptionReflector(); break;
	case TypeId_Uuid: res = co::__createUuidReflector(); break;
	default: assert( false );
	};
	assert( res != NULL );
	return res;
}
