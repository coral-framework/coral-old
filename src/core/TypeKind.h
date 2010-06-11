/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPEKIND_H_
#define _CO_TYPEKIND_H_

namespace co {

enum TypeKind
{
	TK_NONE,
	TK_ANY,
	TK_BOOLEAN,
	TK_INT8,
	TK_UINT8,
	TK_INT16,
	TK_UINT16,
	TK_INT32,
	TK_UINT32,
	TK_INT64,
	TK_UINT64,
	TK_FLOAT,
	TK_DOUBLE,
	TK_STRING,
	TK_ARRAY,
	TK_ENUM,
	TK_EXCEPTION,
	TK_STRUCT,
	TK_NATIVECLASS,
	TK_INTERFACE,
	TK_COMPONENT
};

} // namespace co

#endif
