/*******************************************************************************
** ModuleInstaller class generated for module 'co'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

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
		_reflectors[i]->getType()->setReflector( NULL );
		_reflectors[i] = NULL;
	}
}

// declare all reflector creation functions:
namespace __coral {
	co::Reflector* createArrayTypeReflector();
	co::Reflector* createArrayTypeComponentReflector();
	co::Reflector* createAttributeContainerReflector();
	co::Reflector* createAttributeInfoReflector();
	co::Reflector* createAttributeInfoComponentReflector();
	co::Reflector* createComponentReflector();
	co::Reflector* createComponentTypeReflector();
	co::Reflector* createComponentTypeComponentReflector();
	co::Reflector* createCompoundTypeReflector();
	co::Reflector* createCSLErrorReflector();
	co::Reflector* createDynamicProxyHandlerReflector();
	co::Reflector* createEnumTypeReflector();
	co::Reflector* createEnumTypeComponentReflector();
	co::Reflector* createExceptionTypeReflector();
	co::Reflector* createExceptionTypeComponentReflector();
	co::Reflector* createIllegalArgumentExceptionReflector();
	co::Reflector* createIllegalCastExceptionReflector();
	co::Reflector* createIllegalNameExceptionReflector();
	co::Reflector* createInterfaceReflector();
	co::Reflector* createInterfaceInfoReflector();
	co::Reflector* createInterfaceInfoComponentReflector();
	co::Reflector* createInterfaceTypeReflector();
	co::Reflector* createInterfaceTypeComponentReflector();
	co::Reflector* createLifeCycleExceptionReflector();
	co::Reflector* createMemberInfoReflector();
	co::Reflector* createMethodBuilderReflector();
	co::Reflector* createMethodBuilderComponentReflector();
	co::Reflector* createMethodContainerReflector();
	co::Reflector* createMethodInfoReflector();
	co::Reflector* createMethodInfoComponentReflector();
	co::Reflector* createMissingInputExceptionReflector();
	co::Reflector* createMissingServiceExceptionReflector();
	co::Reflector* createModuleReflector();
	co::Reflector* createModuleComponentReflector();
	co::Reflector* createModuleLoadExceptionReflector();
	co::Reflector* createModuleManagerReflector();
	co::Reflector* createModuleManagerComponentReflector();
	co::Reflector* createModulePartReflector();
	co::Reflector* createModulePartLoaderReflector();
	co::Reflector* createModulePartLoaderComponentReflector();
	co::Reflector* createModuleStateReflector();
	co::Reflector* createNamespaceReflector();
	co::Reflector* createNamespaceComponentReflector();
	co::Reflector* createNativeClassTypeReflector();
	co::Reflector* createNativeClassTypeComponentReflector();
	co::Reflector* createNoSuchInterfaceExceptionReflector();
	co::Reflector* createParameterInfoReflector();
	co::Reflector* createParameterInfoComponentReflector();
	co::Reflector* createReflectorReflector();
	co::Reflector* createSemanticExceptionReflector();
	co::Reflector* createStructTypeReflector();
	co::Reflector* createStructTypeComponentReflector();
	co::Reflector* createSystemReflector();
	co::Reflector* createSystemComponentReflector();
	co::Reflector* createSystemStateReflector();
	co::Reflector* createTypeReflector();
	co::Reflector* createTypeBuilderReflector();
	co::Reflector* createTypeBuilderComponentReflector();
	co::Reflector* createTypeComponentReflector();
	co::Reflector* createTypeCreationTransactionReflector();
	co::Reflector* createTypeCreationTransactionComponentReflector();
	co::Reflector* createTypeKindReflector();
	co::Reflector* createTypeLoadExceptionReflector();
	co::Reflector* createTypeManagerReflector();
	co::Reflector* createTypeManagerComponentReflector();
	co::Reflector* createUnsupportedOperationExceptionReflector();
	co::Reflector* createUuidReflector();
} // namespace __coral

co::Reflector* co::ModuleInstaller::createReflector( TypeId typeId )
{
	co::Reflector* res = NULL;
	switch( typeId )
	{
	case TypeId_ArrayType: res = __coral::createArrayTypeReflector(); break;
	case TypeId_ArrayTypeComponent: res = __coral::createArrayTypeComponentReflector(); break;
	case TypeId_AttributeContainer: res = __coral::createAttributeContainerReflector(); break;
	case TypeId_AttributeInfo: res = __coral::createAttributeInfoReflector(); break;
	case TypeId_AttributeInfoComponent: res = __coral::createAttributeInfoComponentReflector(); break;
	case TypeId_Component: res = __coral::createComponentReflector(); break;
	case TypeId_ComponentType: res = __coral::createComponentTypeReflector(); break;
	case TypeId_ComponentTypeComponent: res = __coral::createComponentTypeComponentReflector(); break;
	case TypeId_CompoundType: res = __coral::createCompoundTypeReflector(); break;
	case TypeId_CSLError: res = __coral::createCSLErrorReflector(); break;
	case TypeId_DynamicProxyHandler: res = __coral::createDynamicProxyHandlerReflector(); break;
	case TypeId_EnumType: res = __coral::createEnumTypeReflector(); break;
	case TypeId_EnumTypeComponent: res = __coral::createEnumTypeComponentReflector(); break;
	case TypeId_ExceptionType: res = __coral::createExceptionTypeReflector(); break;
	case TypeId_ExceptionTypeComponent: res = __coral::createExceptionTypeComponentReflector(); break;
	case TypeId_IllegalArgumentException: res = __coral::createIllegalArgumentExceptionReflector(); break;
	case TypeId_IllegalCastException: res = __coral::createIllegalCastExceptionReflector(); break;
	case TypeId_IllegalNameException: res = __coral::createIllegalNameExceptionReflector(); break;
	case TypeId_Interface: res = __coral::createInterfaceReflector(); break;
	case TypeId_InterfaceInfo: res = __coral::createInterfaceInfoReflector(); break;
	case TypeId_InterfaceInfoComponent: res = __coral::createInterfaceInfoComponentReflector(); break;
	case TypeId_InterfaceType: res = __coral::createInterfaceTypeReflector(); break;
	case TypeId_InterfaceTypeComponent: res = __coral::createInterfaceTypeComponentReflector(); break;
	case TypeId_LifeCycleException: res = __coral::createLifeCycleExceptionReflector(); break;
	case TypeId_MemberInfo: res = __coral::createMemberInfoReflector(); break;
	case TypeId_MethodBuilder: res = __coral::createMethodBuilderReflector(); break;
	case TypeId_MethodBuilderComponent: res = __coral::createMethodBuilderComponentReflector(); break;
	case TypeId_MethodContainer: res = __coral::createMethodContainerReflector(); break;
	case TypeId_MethodInfo: res = __coral::createMethodInfoReflector(); break;
	case TypeId_MethodInfoComponent: res = __coral::createMethodInfoComponentReflector(); break;
	case TypeId_MissingInputException: res = __coral::createMissingInputExceptionReflector(); break;
	case TypeId_MissingServiceException: res = __coral::createMissingServiceExceptionReflector(); break;
	case TypeId_Module: res = __coral::createModuleReflector(); break;
	case TypeId_ModuleComponent: res = __coral::createModuleComponentReflector(); break;
	case TypeId_ModuleLoadException: res = __coral::createModuleLoadExceptionReflector(); break;
	case TypeId_ModuleManager: res = __coral::createModuleManagerReflector(); break;
	case TypeId_ModuleManagerComponent: res = __coral::createModuleManagerComponentReflector(); break;
	case TypeId_ModulePart: res = __coral::createModulePartReflector(); break;
	case TypeId_ModulePartLoader: res = __coral::createModulePartLoaderReflector(); break;
	case TypeId_ModulePartLoaderComponent: res = __coral::createModulePartLoaderComponentReflector(); break;
	case TypeId_ModuleState: res = __coral::createModuleStateReflector(); break;
	case TypeId_Namespace: res = __coral::createNamespaceReflector(); break;
	case TypeId_NamespaceComponent: res = __coral::createNamespaceComponentReflector(); break;
	case TypeId_NativeClassType: res = __coral::createNativeClassTypeReflector(); break;
	case TypeId_NativeClassTypeComponent: res = __coral::createNativeClassTypeComponentReflector(); break;
	case TypeId_NoSuchInterfaceException: res = __coral::createNoSuchInterfaceExceptionReflector(); break;
	case TypeId_ParameterInfo: res = __coral::createParameterInfoReflector(); break;
	case TypeId_ParameterInfoComponent: res = __coral::createParameterInfoComponentReflector(); break;
	case TypeId_Reflector: res = __coral::createReflectorReflector(); break;
	case TypeId_SemanticException: res = __coral::createSemanticExceptionReflector(); break;
	case TypeId_StructType: res = __coral::createStructTypeReflector(); break;
	case TypeId_StructTypeComponent: res = __coral::createStructTypeComponentReflector(); break;
	case TypeId_System: res = __coral::createSystemReflector(); break;
	case TypeId_SystemComponent: res = __coral::createSystemComponentReflector(); break;
	case TypeId_SystemState: res = __coral::createSystemStateReflector(); break;
	case TypeId_Type: res = __coral::createTypeReflector(); break;
	case TypeId_TypeBuilder: res = __coral::createTypeBuilderReflector(); break;
	case TypeId_TypeBuilderComponent: res = __coral::createTypeBuilderComponentReflector(); break;
	case TypeId_TypeComponent: res = __coral::createTypeComponentReflector(); break;
	case TypeId_TypeCreationTransaction: res = __coral::createTypeCreationTransactionReflector(); break;
	case TypeId_TypeCreationTransactionComponent: res = __coral::createTypeCreationTransactionComponentReflector(); break;
	case TypeId_TypeKind: res = __coral::createTypeKindReflector(); break;
	case TypeId_TypeLoadException: res = __coral::createTypeLoadExceptionReflector(); break;
	case TypeId_TypeManager: res = __coral::createTypeManagerReflector(); break;
	case TypeId_TypeManagerComponent: res = __coral::createTypeManagerComponentReflector(); break;
	case TypeId_UnsupportedOperationException: res = __coral::createUnsupportedOperationExceptionReflector(); break;
	case TypeId_Uuid: res = __coral::createUuidReflector(); break;
	default: assert( false );
	};
	assert( res != NULL );
	return res;
}
