/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/RefPtr.h>
#include <co/Reflector.h>
#include <co/ModulePart.h>

namespace co {

class ModuleInstaller
{
public:
	inline static ModuleInstaller& instance() { return sm_instance; }

	enum TypeId
	{
		TypeId_ArrayType,
		TypeId_ArrayTypeComponent,
		TypeId_AttributeContainer,
		TypeId_AttributeInfo,
		TypeId_AttributeInfoComponent,
		TypeId_Component,
		TypeId_ComponentType,
		TypeId_ComponentTypeComponent,
		TypeId_CompoundType,
		TypeId_CSLError,
		TypeId_DynamicProxyHandler,
		TypeId_EnumType,
		TypeId_EnumTypeComponent,
		TypeId_ExceptionType,
		TypeId_ExceptionTypeComponent,
		TypeId_IllegalArgumentException,
		TypeId_IllegalCastException,
		TypeId_IllegalNameException,
		TypeId_Interface,
		TypeId_InterfaceInfo,
		TypeId_InterfaceInfoComponent,
		TypeId_InterfaceType,
		TypeId_InterfaceTypeComponent,
		TypeId_LifeCycleException,
		TypeId_MemberInfo,
		TypeId_MethodBuilder,
		TypeId_MethodBuilderComponent,
		TypeId_MethodContainer,
		TypeId_MethodInfo,
		TypeId_MethodInfoComponent,
		TypeId_MissingInputException,
		TypeId_MissingServiceException,
		TypeId_Module,
		TypeId_ModuleComponent,
		TypeId_ModuleLoadException,
		TypeId_ModuleManager,
		TypeId_ModuleManagerComponent,
		TypeId_ModulePart,
		TypeId_ModulePartLoader,
		TypeId_ModulePartLoaderComponent,
		TypeId_ModuleState,
		TypeId_Namespace,
		TypeId_NamespaceComponent,
		TypeId_NativeClassType,
		TypeId_NativeClassTypeComponent,
		TypeId_NoSuchInterfaceException,
		TypeId_NotSupportedException,
		TypeId_ParameterInfo,
		TypeId_ParameterInfoComponent,
		TypeId_Reflector,
		TypeId_SemanticException,
		TypeId_ServiceManager,
		TypeId_ServiceManagerComponent,
		TypeId_StructType,
		TypeId_StructTypeComponent,
		TypeId_System,
		TypeId_SystemComponent,
		TypeId_SystemState,
		TypeId_Type,
		TypeId_TypeBuilder,
		TypeId_TypeBuilderComponent,
		TypeId_TypeComponent,
		TypeId_TypeCreationTransaction,
		TypeId_TypeCreationTransactionComponent,
		TypeId_TypeKind,
		TypeId_TypeLoadException,
		TypeId_TypeManager,
		TypeId_TypeManagerComponent,
		TypeId_Uuid,
		TypeCount
	};

public:
	//! Instantiates Reflectors for the module types that still don't have an assigned Reflector.
	void initialize();

	//! Returns the Reflector that should be assigned to a module type.
	inline co::Reflector* getReflector( TypeId typeId ) { return _reflectors[typeId].get(); }

	//! Sets the Reflector instance that should be assigned to module type.
	//! \warning This method can only be called \b before calling install().
	inline void setReflector( TypeId typeId, co::Reflector* reflector ) { _reflectors[typeId] = reflector; }

	//! Assigns the Reflectors to their corresponding co::Type instances.
	//! This method skips any co::Type instance that already has an assigned Reflector.
	void install();

	//! Removes references to the Reflectors from their corresponding co::Type instances.
	//! This ignores any co::Type instance whose current Reflector was not assigned by this class.
	void uninstall();

private:
	ModuleInstaller();
	~ModuleInstaller();

	co::Reflector* createReflector( TypeId typeId );

private:
	static ModuleInstaller sm_instance;

private:
	co::RefPtr<co::Reflector> _reflectors[TypeCount];
};

// Module's reflector creation functions:
co::Reflector* __createArrayTypeReflector();
co::Reflector* __createArrayTypeComponentReflector();
co::Reflector* __createAttributeContainerReflector();
co::Reflector* __createAttributeInfoReflector();
co::Reflector* __createAttributeInfoComponentReflector();
co::Reflector* __createComponentReflector();
co::Reflector* __createComponentTypeReflector();
co::Reflector* __createComponentTypeComponentReflector();
co::Reflector* __createCompoundTypeReflector();
co::Reflector* __createCSLErrorReflector();
co::Reflector* __createDynamicProxyHandlerReflector();
co::Reflector* __createEnumTypeReflector();
co::Reflector* __createEnumTypeComponentReflector();
co::Reflector* __createExceptionTypeReflector();
co::Reflector* __createExceptionTypeComponentReflector();
co::Reflector* __createIllegalArgumentExceptionReflector();
co::Reflector* __createIllegalCastExceptionReflector();
co::Reflector* __createIllegalNameExceptionReflector();
co::Reflector* __createInterfaceReflector();
co::Reflector* __createInterfaceInfoReflector();
co::Reflector* __createInterfaceInfoComponentReflector();
co::Reflector* __createInterfaceTypeReflector();
co::Reflector* __createInterfaceTypeComponentReflector();
co::Reflector* __createLifeCycleExceptionReflector();
co::Reflector* __createMemberInfoReflector();
co::Reflector* __createMethodBuilderReflector();
co::Reflector* __createMethodBuilderComponentReflector();
co::Reflector* __createMethodContainerReflector();
co::Reflector* __createMethodInfoReflector();
co::Reflector* __createMethodInfoComponentReflector();
co::Reflector* __createMissingInputExceptionReflector();
co::Reflector* __createMissingServiceExceptionReflector();
co::Reflector* __createModuleReflector();
co::Reflector* __createModuleComponentReflector();
co::Reflector* __createModuleLoadExceptionReflector();
co::Reflector* __createModuleManagerReflector();
co::Reflector* __createModuleManagerComponentReflector();
co::Reflector* __createModulePartReflector();
co::Reflector* __createModulePartLoaderReflector();
co::Reflector* __createModulePartLoaderComponentReflector();
co::Reflector* __createModuleStateReflector();
co::Reflector* __createNamespaceReflector();
co::Reflector* __createNamespaceComponentReflector();
co::Reflector* __createNativeClassTypeReflector();
co::Reflector* __createNativeClassTypeComponentReflector();
co::Reflector* __createNoSuchInterfaceExceptionReflector();
co::Reflector* __createNotSupportedExceptionReflector();
co::Reflector* __createParameterInfoReflector();
co::Reflector* __createParameterInfoComponentReflector();
co::Reflector* __createReflectorReflector();
co::Reflector* __createSemanticExceptionReflector();
co::Reflector* __createServiceManagerReflector();
co::Reflector* __createServiceManagerComponentReflector();
co::Reflector* __createStructTypeReflector();
co::Reflector* __createStructTypeComponentReflector();
co::Reflector* __createSystemReflector();
co::Reflector* __createSystemComponentReflector();
co::Reflector* __createSystemStateReflector();
co::Reflector* __createTypeReflector();
co::Reflector* __createTypeBuilderReflector();
co::Reflector* __createTypeBuilderComponentReflector();
co::Reflector* __createTypeComponentReflector();
co::Reflector* __createTypeCreationTransactionReflector();
co::Reflector* __createTypeCreationTransactionComponentReflector();
co::Reflector* __createTypeKindReflector();
co::Reflector* __createTypeLoadExceptionReflector();
co::Reflector* __createTypeManagerReflector();
co::Reflector* __createTypeManagerComponentReflector();
co::Reflector* __createUuidReflector();

} // namespace co
