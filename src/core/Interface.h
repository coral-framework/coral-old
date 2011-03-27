/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _INTERFACETYPE_H_
#define _INTERFACETYPE_H_

#include "CompositeType.h"
#include "Interface_Base.h"

namespace co {

/*!
	Implements co.IInterface.
 */
class Interface : public Interface_Base, public CompositeTypeImpl
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

	DELEGATE_co_IType( CompositeTypeImpl:: );
	DELEGATE_co_ICompositeType( CompositeTypeImpl:: );
	DELEGATE_co_IRecordType( CompositeTypeImpl:: );
	DELEGATE_co_IClassType( CompositeTypeImpl:: );

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
