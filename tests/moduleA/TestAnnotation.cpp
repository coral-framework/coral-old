/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "TestAnnotation_Base.h"
#include <moduleA/Vec2D.h>
#include <co/Any.h>
#include <co/IType.h>
#include <co/CSLError.h>
#include <co/reserved/Uuid.h>

namespace moduleA {

class TestAnnotation : public TestAnnotation_Base
{
public:
	co::AnyValue getA() { return _any; }
	void setA( const co::Any& a ) { _any = a; }

	bool getB() { return _b; }
	void setB( bool b ) { _b = b; }

	co::CSLError getCslError() { return _cslError; }
	void setCslError( const co::CSLError& cslError ) { _cslError = cslError; }

	double getDbl() { return _dbl; }
	void setDbl( double dbl ) { _dbl = dbl; }

	co::TSlice<double> getDblArray()
	{
		return _dblArray;
	}

	void setDblArray( co::Slice<double> dblArray )
	{
		co::assign( dblArray, _dblArray );
	}

	float getFlt() { return _flt; }
	void setFlt( float flt ) { _flt = flt; }

	co::int16 getI16() { return _i16; }
	void setI16( co::int16 i16 ) { _i16 = i16; }

	co::int32 getI32() { return _i32; }
	void setI32( co::int32 i32 ) { _i32 = i32; }

	co::int8 getI8() { return _i8; }
	void setI8( co::int8 i8 ) { _i8 = i8; }

	double getReadOnlyDbl() { return 3.14; }
	std::string getReadOnlyStr()
	{
		static std::string s_str( "My read-only string" );
		return s_str;
	}

	std::string getStr() { return _str; }
	void setStr( const std::string& str ) { _str = str; }

	co::TSlice<std::string> getStrArray()
	{
		return _strArray;
	}

	void setStrArray( co::Slice<std::string> strArray )
	{
		co::assign( strArray, _strArray );
	}

	co::IType* getType() { return _type; }
	void setType( co::IType* type ) { _type = type; }

	co::TypeKind getTypeKind() { return _typeKind; }
	void setTypeKind( co::TypeKind typeKind ) { _typeKind = typeKind; }

	co::uint16 getU16() { return _u16; }
	void setU16( co::uint16 u16 ) { _u16 = u16; }

	co::uint32 getU32() { return _u32; }
	void setU32( co::uint32 u32 ) { _u32 = u32; }

	co::uint8 getU8() { return _u8; }
	void setU8( co::uint8 u8 ) { _u8 = u8; }

	co::Uuid getUuid() { return _uuid; }
	void setUuid( const co::Uuid& uuid ) { _uuid = uuid; }

	Vec2D getVec2D() { return _vec2d; }
	void setVec2D( const Vec2D& vec2d ) { _vec2d = vec2d; }

	double getValue() { return _value; }
	void setValue( double value ) { _value = value; }

private:
	double _value;

	co::AnyValue _any;
	bool _b;
	co::int8 _i8;
	co::uint8 _u8;
	co::int16 _i16;
	co::uint16 _u16;
	co::int32 _i32;
	co::uint32 _u32;
	float _flt;
	double _dbl;
	std::string _str;

	std::vector<std::string> _strArray;
	std::vector<double> _dblArray;

	co::TypeKind _typeKind;
	co::CSLError _cslError;
	co::Uuid _uuid;
	Vec2D _vec2d;

	co::IType* _type;
};

CORAL_EXPORT_COMPONENT( TestAnnotation, TestAnnotation );

} // namespace moduleA
