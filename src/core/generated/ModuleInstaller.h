/*******************************************************************************
** ModuleInstaller class generated for module 'co'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

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
		TypeId_UnsupportedOperationException,
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

} // namespace co
