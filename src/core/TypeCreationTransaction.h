/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPECREATIONTRANSACTION_H_
#define _TYPECREATIONTRANSACTION_H_

#include "TypeCreationTransactionComponent_Base.h"
#include <co/RefVector.h>
#include <co/TypeBuilder.h>

namespace co {

/*!
	Component that implements co.TypeCreationTransaction.
 */
class TypeCreationTransactionComponent
		: public TypeCreationTransactionComponent_Base
{
public:
	TypeCreationTransactionComponent();
	virtual ~TypeCreationTransactionComponent();

	// internal methods:
	void addTypeBuilder( TypeBuilder* typeBuilder );

	// TypeCreationTransaction methods:
	ArrayRange<TypeBuilder* const> getTypeBuilders();
	void commit();
	void rollback();
	
private:
	static TypeCreationTransactionComponent* sm_activeTransaction;

private:
	bool _commitAttempted;
	bool _commitSucceeded;
	bool _rolledBack;

	typedef RefVector<TypeBuilder> TypeBuilderList;
	TypeBuilderList _typeBuilders;
};

} // namespace co

#endif
