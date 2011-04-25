/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_CSLERROR_H_
#define _CO_CSLERROR_H_

#include <co/TypeTraits.h>

// co.CSLError Mapping:
namespace co {

struct CSLError
{
	std::string filename; // estimated size of 8 bytes
	std::string message; // estimated size of 8 bytes
	co::int32 line; // estimated size of 4 bytes

	//! Default constructor.
	CSLError() :
		filename(),
		message(),
		line()
	{;}

	//! Equality test operator.
	inline bool operator==( const CSLError& o ) const
	{
		return (
			filename == o.filename &&
			message == o.message &&
			line == o.line
		);
	}

	//! Inequality test operator.
	inline bool operator!=( const CSLError& o ) const { return !( *this == o ); }

	//! ADL-based overload for swap() calls.
	friend inline void swap( CSLError& a, CSLError& b )
	{
		using std::swap;
		swap( a.filename, b.filename );
		swap( a.message, b.message );
		swap( a.line, b.line );
	}
};

} // namespace co

namespace co {
template<> struct kindOf<co::CSLError> : public kindOfBase<TK_STRUCT> {};
template<> struct nameOf<co::CSLError> { static const char* get() { return "co.CSLError"; } };
template<> struct typeOf<co::CSLError> : public typeOfBase<co::CSLError, IStruct> {};
} // namespace co

#endif // _CO_CSLERROR_H_
