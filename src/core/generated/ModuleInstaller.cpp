/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModuleInstaller.h"
#include <co/IType.h>

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
		co::IReflector* reflector = _reflectors[i].get();
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

co::IReflector* co::ModuleInstaller::createReflector( TypeId typeId )
{
	co::IReflector* res = NULL;
	switch( typeId )
	{
	case TypeId_ArrayType: res = co::__createArrayTypeIReflector(); break;
	case TypeId_AttributeInfo: res = co::__createAttributeInfoIReflector(); break;
	case TypeId_ComponentType: res = co::__createComponentTypeIReflector(); break;
	case TypeId_CSLError: res = co::__createCSLErrorIReflector(); break;
	case TypeId_EnumType: res = co::__createEnumTypeIReflector(); break;
	case TypeId_ExceptionType: res = co::__createExceptionTypeIReflector(); break;
	case TypeId_IArrayType: res = co::__createIArrayTypeIReflector(); break;
	case TypeId_IAttributeContainer: res = co::__createIAttributeContainerIReflector(); break;
	case TypeId_IAttributeInfo: res = co::__createIAttributeInfoIReflector(); break;
	case TypeId_IComponent: res = co::__createIComponentIReflector(); break;
	case TypeId_IComponentType: res = co::__createIComponentTypeIReflector(); break;
	case TypeId_ICompoundType: res = co::__createICompoundTypeIReflector(); break;
	case TypeId_IDynamicProxyHandler: res = co::__createIDynamicProxyHandlerIReflector(); break;
	case TypeId_IEnumType: res = co::__createIEnumTypeIReflector(); break;
	case TypeId_IExceptionType: res = co::__createIExceptionTypeIReflector(); break;
	case TypeId_IInterfaceInfo: res = co::__createIInterfaceInfoIReflector(); break;
	case TypeId_IInterfaceType: res = co::__createIInterfaceTypeIReflector(); break;
	case TypeId_IllegalArgumentException: res = co::__createIllegalArgumentExceptionIReflector(); break;
	case TypeId_IllegalCastException: res = co::__createIllegalCastExceptionIReflector(); break;
	case TypeId_IllegalNameException: res = co::__createIllegalNameExceptionIReflector(); break;
	case TypeId_IMemberInfo: res = co::__createIMemberInfoIReflector(); break;
	case TypeId_IMethodBuilder: res = co::__createIMethodBuilderIReflector(); break;
	case TypeId_IMethodContainer: res = co::__createIMethodContainerIReflector(); break;
	case TypeId_IMethodInfo: res = co::__createIMethodInfoIReflector(); break;
	case TypeId_IModule: res = co::__createIModuleIReflector(); break;
	case TypeId_IModuleManager: res = co::__createIModuleManagerIReflector(); break;
	case TypeId_IModulePart: res = co::__createIModulePartIReflector(); break;
	case TypeId_IModulePartLoader: res = co::__createIModulePartLoaderIReflector(); break;
	case TypeId_INamespace: res = co::__createINamespaceIReflector(); break;
	case TypeId_INativeClassType: res = co::__createINativeClassTypeIReflector(); break;
	case TypeId_Interface: res = co::__createInterfaceIReflector(); break;
	case TypeId_InterfaceInfo: res = co::__createInterfaceInfoIReflector(); break;
	case TypeId_InterfaceType: res = co::__createInterfaceTypeIReflector(); break;
	case TypeId_IParameterInfo: res = co::__createIParameterInfoIReflector(); break;
	case TypeId_IReflector: res = co::__createIReflectorIReflector(); break;
	case TypeId_IServiceManager: res = co::__createIServiceManagerIReflector(); break;
	case TypeId_IStructType: res = co::__createIStructTypeIReflector(); break;
	case TypeId_ISystem: res = co::__createISystemIReflector(); break;
	case TypeId_IType: res = co::__createITypeIReflector(); break;
	case TypeId_ITypeBuilder: res = co::__createITypeBuilderIReflector(); break;
	case TypeId_ITypeCreationTransaction: res = co::__createITypeCreationTransactionIReflector(); break;
	case TypeId_ITypeManager: res = co::__createITypeManagerIReflector(); break;
	case TypeId_LifeCycleException: res = co::__createLifeCycleExceptionIReflector(); break;
	case TypeId_MethodBuilder: res = co::__createMethodBuilderIReflector(); break;
	case TypeId_MethodInfo: res = co::__createMethodInfoIReflector(); break;
	case TypeId_MissingInputException: res = co::__createMissingInputExceptionIReflector(); break;
	case TypeId_MissingServiceException: res = co::__createMissingServiceExceptionIReflector(); break;
	case TypeId_Module: res = co::__createModuleIReflector(); break;
	case TypeId_ModuleLoadException: res = co::__createModuleLoadExceptionIReflector(); break;
	case TypeId_ModuleManager: res = co::__createModuleManagerIReflector(); break;
	case TypeId_ModulePartLoader: res = co::__createModulePartLoaderIReflector(); break;
	case TypeId_ModuleState: res = co::__createModuleStateIReflector(); break;
	case TypeId_Namespace: res = co::__createNamespaceIReflector(); break;
	case TypeId_NativeClassType: res = co::__createNativeClassTypeIReflector(); break;
	case TypeId_NoSuchInterfaceException: res = co::__createNoSuchInterfaceExceptionIReflector(); break;
	case TypeId_NotSupportedException: res = co::__createNotSupportedExceptionIReflector(); break;
	case TypeId_ParameterInfo: res = co::__createParameterInfoIReflector(); break;
	case TypeId_SemanticException: res = co::__createSemanticExceptionIReflector(); break;
	case TypeId_ServiceManager: res = co::__createServiceManagerIReflector(); break;
	case TypeId_StructType: res = co::__createStructTypeIReflector(); break;
	case TypeId_System: res = co::__createSystemIReflector(); break;
	case TypeId_SystemState: res = co::__createSystemStateIReflector(); break;
	case TypeId_Type: res = co::__createTypeIReflector(); break;
	case TypeId_TypeBuilder: res = co::__createTypeBuilderIReflector(); break;
	case TypeId_TypeCreationTransaction: res = co::__createTypeCreationTransactionIReflector(); break;
	case TypeId_TypeKind: res = co::__createTypeKindIReflector(); break;
	case TypeId_TypeLoadException: res = co::__createTypeLoadExceptionIReflector(); break;
	case TypeId_TypeManager: res = co::__createTypeManagerIReflector(); break;
	case TypeId_Uuid: res = co::__createUuidIReflector(); break;
	default: assert( false );
	};
	assert( res != NULL );
	return res;
}
