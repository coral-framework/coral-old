/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _TYPECREATIONTRANSACTION_H_
#define _TYPECREATIONTRANSACTION_H_

#include "TypeCreationTransactionComponent_Base.h"
#include <co/RefVector.h>
#include <co/TypeBuilder.h>

/*!
	Component that implements co.TypeCreationTransaction.
 */
class TypeCreationTransaction : public co::TypeCreationTransactionComponent_Base
{
public:
	TypeCreationTransaction();
	virtual ~TypeCreationTransaction();

	// internal methods:
	void addTypeBuilder( co::TypeBuilder* typeBuilder );

	// co::TypeCreationTransaction methods:
	co::ArrayRange<co::TypeBuilder* const> getTypeBuilders();
	void commit();
	void rollback();
	
private:
	static TypeCreationTransaction* sm_activeTransaction;

private:
	bool _commitAttempted;
	bool _commitSucceeded;
	bool _rolledBack;

	typedef co::RefVector<co::TypeBuilder> TypeBuilderList;
	TypeBuilderList _typeBuilders;
};

#endif
