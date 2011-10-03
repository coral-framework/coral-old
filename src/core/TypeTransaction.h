/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPETRANSACTION_H_
#define _CO_TYPETRANSACTION_H_

#include "TypeTransaction_Base.h"
#include <co/RefVector.h>
#include <co/ITypeBuilder.h>

namespace co {

/*!
	Implements co.ITypeTransaction.
 */
class TypeTransaction : public TypeTransaction_Base
{
public:
	TypeTransaction();
	virtual ~TypeTransaction();

	// internal methods:
	void addTypeBuilder( ITypeBuilder* typeBuilder );

	// ITypeTransaction methods:
	Range<ITypeBuilder* const> getTypeBuilders();
	void commit();
	void rollback();
	
private:
	static TypeTransaction* sm_activeTransaction;

private:
	bool _commitAttempted;
	bool _commitSucceeded;
	bool _rolledBack;

	typedef RefVector<ITypeBuilder> TypeBuilderList;
	TypeBuilderList _typeBuilders;
};

} // namespace co

#endif
