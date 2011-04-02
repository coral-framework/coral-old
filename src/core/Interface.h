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
	Interface();
	virtual ~Interface();

	// internal methods:
	void setBaseType( IInterface* base );
	void addSubType( IInterface* sub );
	void updateSuperTypes();

	// ICompositeType methods:
	Range<IMember* const> getMembers();
	IMember* getMember( const std::string& name );

	// IInterface methods:
	IInterface* getBaseType();
	Range<IInterface* const> getSuperTypes();
	Range<IInterface* const> getSubTypes();
	const std::string& getCppBlock();
	bool isSubTypeOf( IInterface* type );

	DELEGATE_co_IType( ClassTypeImpl:: );
	DELEGATE_co_IRecordType( ClassTypeImpl:: );
	DELEGATE_co_IClassType( ClassTypeImpl:: );

private:
	IInterface* _baseType;
	size_t _numSuperTypes;
	IInterface** _superTypes;
	std::vector<IInterface*> _subTypes;
};

} // namespace co

#endif
