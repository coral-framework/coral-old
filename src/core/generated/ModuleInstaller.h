/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/RefPtr.h>
#include <co/IReflector.h>
#include <co/IModulePart.h>

namespace co {

class ModuleInstaller
{
public:
	inline static ModuleInstaller& instance() { return sm_instance; }

	enum TypeId
	{
		TypeId_ArrayType,
		TypeId_AttributeInfo,
		TypeId_ComponentType,
		TypeId_CSLError,
		TypeId_EnumType,
		TypeId_ExceptionType,
		TypeId_IArrayType,
		TypeId_IAttributeContainer,
		TypeId_IAttributeInfo,
		TypeId_IComponent,
		TypeId_IComponentType,
		TypeId_ICompoundType,
		TypeId_IDynamicProxyHandler,
		TypeId_IEnumType,
		TypeId_IExceptionType,
		TypeId_IInterfaceInfo,
		TypeId_IInterfaceType,
		TypeId_IllegalArgumentException,
		TypeId_IllegalCastException,
		TypeId_IllegalNameException,
		TypeId_IMemberInfo,
		TypeId_IMethodBuilder,
		TypeId_IMethodContainer,
		TypeId_IMethodInfo,
		TypeId_IModule,
		TypeId_IModuleManager,
		TypeId_IModulePart,
		TypeId_IModulePartLoader,
		TypeId_INamespace,
		TypeId_INativeClassType,
		TypeId_Interface,
		TypeId_InterfaceInfo,
		TypeId_InterfaceType,
		TypeId_IParameterInfo,
		TypeId_IReflector,
		TypeId_IServiceManager,
		TypeId_IStructType,
		TypeId_ISystem,
		TypeId_IType,
		TypeId_ITypeBuilder,
		TypeId_ITypeCreationTransaction,
		TypeId_ITypeManager,
		TypeId_LifeCycleException,
		TypeId_MethodBuilder,
		TypeId_MethodInfo,
		TypeId_MissingInputException,
		TypeId_MissingServiceException,
		TypeId_Module,
		TypeId_ModuleLoadException,
		TypeId_ModuleManager,
		TypeId_ModulePartLoader,
		TypeId_ModuleState,
		TypeId_Namespace,
		TypeId_NativeClassType,
		TypeId_NoSuchInterfaceException,
		TypeId_NotSupportedException,
		TypeId_ParameterInfo,
		TypeId_SemanticException,
		TypeId_ServiceManager,
		TypeId_StructType,
		TypeId_System,
		TypeId_SystemState,
		TypeId_Type,
		TypeId_TypeBuilder,
		TypeId_TypeCreationTransaction,
		TypeId_TypeKind,
		TypeId_TypeLoadException,
		TypeId_TypeManager,
		TypeId_Uuid,
		TypeCount
	};

public:
	//! Instantiates Reflectors for the module types that still don't have an assigned IReflector.
	void initialize();

	//! Returns the IReflector that should be assigned to a module type.
	inline co::IReflector* getReflector( TypeId typeId ) { return _reflectors[typeId].get(); }

	//! Sets the IReflector instance that should be assigned to module type.
	//! \warning This method can only be called \b before calling install().
	inline void setReflector( TypeId typeId, co::IReflector* reflector ) { _reflectors[typeId] = reflector; }

	//! Assigns the reflectors to their corresponding co::IType instances.
	//! This method skips any co::IType instance that already has an assigned IReflector.
	void install();

	//! Removes references to the Reflectors from their corresponding co::IType instances.
	//! This ignores any co::IType instance whose current IReflector was not assigned by this class.
	void uninstall();

private:
	ModuleInstaller();
	~ModuleInstaller();

	co::IReflector* createReflector( TypeId typeId );

private:
	static ModuleInstaller sm_instance;

private:
	co::RefPtr<co::IReflector> _reflectors[TypeCount];
};

// Module's reflector creation functions:
co::IReflector* __createArrayTypeIReflector();
co::IReflector* __createAttributeInfoIReflector();
co::IReflector* __createComponentTypeIReflector();
co::IReflector* __createCSLErrorIReflector();
co::IReflector* __createEnumTypeIReflector();
co::IReflector* __createExceptionTypeIReflector();
co::IReflector* __createIArrayTypeIReflector();
co::IReflector* __createIAttributeContainerIReflector();
co::IReflector* __createIAttributeInfoIReflector();
co::IReflector* __createIComponentIReflector();
co::IReflector* __createIComponentTypeIReflector();
co::IReflector* __createICompoundTypeIReflector();
co::IReflector* __createIDynamicProxyHandlerIReflector();
co::IReflector* __createIEnumTypeIReflector();
co::IReflector* __createIExceptionTypeIReflector();
co::IReflector* __createIInterfaceInfoIReflector();
co::IReflector* __createIInterfaceTypeIReflector();
co::IReflector* __createIllegalArgumentExceptionIReflector();
co::IReflector* __createIllegalCastExceptionIReflector();
co::IReflector* __createIllegalNameExceptionIReflector();
co::IReflector* __createIMemberInfoIReflector();
co::IReflector* __createIMethodBuilderIReflector();
co::IReflector* __createIMethodContainerIReflector();
co::IReflector* __createIMethodInfoIReflector();
co::IReflector* __createIModuleIReflector();
co::IReflector* __createIModuleManagerIReflector();
co::IReflector* __createIModulePartIReflector();
co::IReflector* __createIModulePartLoaderIReflector();
co::IReflector* __createINamespaceIReflector();
co::IReflector* __createINativeClassTypeIReflector();
co::IReflector* __createInterfaceIReflector();
co::IReflector* __createInterfaceInfoIReflector();
co::IReflector* __createInterfaceTypeIReflector();
co::IReflector* __createIParameterInfoIReflector();
co::IReflector* __createIReflectorIReflector();
co::IReflector* __createIServiceManagerIReflector();
co::IReflector* __createIStructTypeIReflector();
co::IReflector* __createISystemIReflector();
co::IReflector* __createITypeIReflector();
co::IReflector* __createITypeBuilderIReflector();
co::IReflector* __createITypeCreationTransactionIReflector();
co::IReflector* __createITypeManagerIReflector();
co::IReflector* __createLifeCycleExceptionIReflector();
co::IReflector* __createMethodBuilderIReflector();
co::IReflector* __createMethodInfoIReflector();
co::IReflector* __createMissingInputExceptionIReflector();
co::IReflector* __createMissingServiceExceptionIReflector();
co::IReflector* __createModuleIReflector();
co::IReflector* __createModuleLoadExceptionIReflector();
co::IReflector* __createModuleManagerIReflector();
co::IReflector* __createModulePartLoaderIReflector();
co::IReflector* __createModuleStateIReflector();
co::IReflector* __createNamespaceIReflector();
co::IReflector* __createNativeClassTypeIReflector();
co::IReflector* __createNoSuchInterfaceExceptionIReflector();
co::IReflector* __createNotSupportedExceptionIReflector();
co::IReflector* __createParameterInfoIReflector();
co::IReflector* __createSemanticExceptionIReflector();
co::IReflector* __createServiceManagerIReflector();
co::IReflector* __createStructTypeIReflector();
co::IReflector* __createSystemIReflector();
co::IReflector* __createSystemStateIReflector();
co::IReflector* __createTypeIReflector();
co::IReflector* __createTypeBuilderIReflector();
co::IReflector* __createTypeCreationTransactionIReflector();
co::IReflector* __createTypeKindIReflector();
co::IReflector* __createTypeLoadExceptionIReflector();
co::IReflector* __createTypeManagerIReflector();
co::IReflector* __createUuidIReflector();

} // namespace co
