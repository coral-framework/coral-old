/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPECREATIONTRANSACTION_H_
#define _TYPECREATIONTRANSACTION_H_

#include "TypeCreationTransaction_Base.h"
#include <co/RefVector.h>
#include <co/ITypeBuilder.h>

namespace co {

/*!
	Implements co.ITypeCreationTransaction.
 */
class TypeCreationTransaction
		: public TypeCreationTransaction_Base
{
public:
	TypeCreationTransaction();
	virtual ~TypeCreationTransaction();

	// internal methods:
	void addTypeBuilder( ITypeBuilder* typeBuilder );

	// ITypeCreationTransaction methods:
	ArrayRange<ITypeBuilder* const> getTypeBuilders();
	void commit();
	void rollback();
	
private:
	static TypeCreationTransaction* sm_activeTransaction;

private:
	bool _commitAttempted;
	bool _commitSucceeded;
	bool _rolledBack;

	typedef RefVector<ITypeBuilder> TypeBuilderList;
	TypeBuilderList _typeBuilders;
};

} // namespace co

#endif
