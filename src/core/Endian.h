/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ENDIAN_H_
#define _CO_ENDIAN_H_

#include <co/Config.h>
#include <co/Platform.h>

namespace co {

inline uint16 endianSwap( uint16 x )
{
	return ( x >> 8 ) | ( x << 8 );
}

inline uint32 endianSwap( uint32 x )
{
	return 	( x >> 24 ) | 
			( ( x << 8 ) & 0x00FF0000 ) |
			( ( x >> 8 ) & 0x0000FF00 ) |
			( x << 24 );
}

inline uint64 endianSwap( uint64 x )
{
	return 	( x >> 56 ) | 
			( ( x << 40 ) & 0x00FF000000000000LL ) |
			( ( x << 24 ) & 0x0000FF0000000000LL ) |
			( ( x << 8 )  & 0x000000FF00000000LL ) |
			( ( x >> 8 )  & 0x00000000FF000000LL ) |
			( ( x >> 24 ) & 0x0000000000FF0000LL ) |
			( ( x >> 40 ) & 0x000000000000FF00LL ) |
			( x << 56 );
}

#if CORAL_BYTE_ORDER == CORAL_LITTLE_ENDIAN

	template<typename T>
	inline T fromBigEndian( T v )
	{
		return endianSwap( v );
	}
	
	template<typename T>
	inline T toBigEndian( T v )
	{
		return endianSwap( v );
	}	

	template<typename T>
	inline T fromLittleEndian( T v )
	{
		return v;
	}

	template<typename T>
	inline T toLittleEndian( T v )
	{
		return v;
	}

#else

	template<typename T>
	inline T fromBigEndian( T v )
	{
		return v;
	}
	
	template<typename T>
	inline T toBigEndian( T v )
	{
		return v;
	}

	template<typename T>
	inline T fromLittleEndian( T v )
	{
		return endianSwap( v );
	}

	template<typename T>
	inline T toLittleEndian( T v )
	{
		return endianSwap( v );
	}

#endif

} // namespace co

#endif // _CO_ENDIAN_H_
