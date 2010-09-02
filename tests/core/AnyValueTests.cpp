/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <gtest/gtest.h>
#include <co/AnyValue.h>
#include <vector>


TEST( AnyValueTests, stdVectorMemoryLayout )
{
	/*
		In AnyData we assume some truths about the memory layout of std::vectors.
		This test verifies whether the assumptions are true for the local STL distro.
	
		Basically, the assumption is that the std::vector computes its size() by subtracting
		two pointers (one to the start, another past the end of the array). This is a fairly
		standard implementation choice for std::vectors, and allows Coral to find out how much
		memory is allocated by a std::vector in run-time without knowing its template type.
	 */

	std::vector<double> dblVector( 8 );
	ASSERT_EQ( 8, dblVector.size() );

	std::vector<co::uint8>& bytesVector = *reinterpret_cast<std::vector<co::uint8>*>( &dblVector );
	ASSERT_EQ( 64, bytesVector.size() );
}
