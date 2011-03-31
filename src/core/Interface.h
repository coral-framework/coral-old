/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INTERFACE_H_
#define _CO_INTERFACE_H_

#include "ClassType.h"
#include "Interface_Base.h"

namespace co {

/*!
	Implements co.IInterface.
 */
class Interface : public Interface_Base, public ClassTypeImpl
{
public:
	virtual ~Interface();

	// internal methods:
	void addSuperInterface( IInterface* superItf );
	void addSubInterface( IInterface* subItf );

	// IInterface methods:
	Range<IInterface* const> getInterfaceAncestors();
	Range<IInterface* const> getSuperInterfaces();
	Range<IInterface* const> getSubInterfaces();
	const std::string& getCppBlock();
	bool isSubTypeOf( IInterface* itf );

	DELEGATE_co_IType( ClassTypeImpl:: );
	DELEGATE_co_ICompositeType( ClassTypeImpl:: );
	DELEGATE_co_IRecordType( ClassTypeImpl:: );
	DELEGATE_co_IClassType( ClassTypeImpl:: );

private:
	Range<ICompositeType* const> getCompositeTypeAncestors();

	// ancestors are computed lazily
	void updateAncestors();

private:
	typedef std::vector<IInterface*> InterfaceVector;
	InterfaceVector _ancestors;
	InterfaceVector _superInterfaces;
	InterfaceVector _subInterfaces;
};

} // namespace co

#endif
