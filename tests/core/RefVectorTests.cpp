/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/RefVector.h>
#include <co/IService.h>
#include <co/INamespace.h>
#include <gtest/gtest.h>

namespace {

class PseudoInterface : public co::IService
{
public:
	PseudoInterface( co::int32 key, bool* setToTrueWhenDestroyed = 0 ) :
		_key( key ), _refCount( 0 ), _setToTrueWhenDestroyed( setToTrueWhenDestroyed )
	{;}

	virtual ~PseudoInterface()
	{
		if( _setToTrueWhenDestroyed )
			*_setToTrueWhenDestroyed = true;
	}

	inline co::int32 getKey() { return _key; }

	co::IInterface* getInterface() { return 0; }
	co::IObject* getProvider() { return 0; }
	co::IPort* getFacet() { return 0; }
	void serviceRetain() { ++_refCount; }
	void serviceRelease() { if( --_refCount <= 0 ) delete this; }

private:
	co::int32 _key;
	co::int32 _refCount;
	bool* _setToTrueWhenDestroyed;
};

} // anonymous namespace

TEST( RefVectorTests, rawPtrVectorEquivalence )
{
	PseudoInterface o1( 1 ), o2( 2 ), o3( 3 );
	o1.serviceRetain();
	o2.serviceRetain();
	o3.serviceRetain();

	co::RefPtr<PseudoInterface> refPtr( &o1 );
	EXPECT_EQ( sizeof( co::RefPtr<PseudoInterface> ), sizeof( PseudoInterface* ) );

	PseudoInterface* rawVec[] = { &o1, &o2, &o3 };

	co::RefVector<PseudoInterface> refVec;
	refVec.push_back( &o1 );
	refVec.push_back( &o2 );
	refVec.push_back( &o3 );

	co::Range<PseudoInterface* const> range( refVec );
	for( int i = 0; i < 3; ++i )
	{
		EXPECT_EQ( range.getFirst(), rawVec[i] );
		range.popFirst();
	}
}

TEST( RefVectorTests, rawPtrVectorWithEmptyRefVector )
{
	co::RefVector<PseudoInterface> emptyRefVec;

	EXPECT_EQ( emptyRefVec.size(), 0 );

	co::Range<PseudoInterface* const> range( emptyRefVec );
	EXPECT_TRUE( range.isEmpty() );
}

TEST( RefVectorTests, castedPtrRange )
{
	PseudoInterface o1( 1 ), o2( 2 ), o3( 3 );
	o1.serviceRetain();
	o2.serviceRetain();
	o3.serviceRetain();

	co::RefVector<PseudoInterface> refVec;
	refVec.push_back( &o1 );
	refVec.push_back( &o2 );
	refVec.push_back( &o3 );

	co::Range<co::IService* const> range( refVec );
	EXPECT_EQ( range.getSize(), 3 );

	// invalid usage sample (causes compile-time error):
	//co::Range<co::INamespace*> invalidRange( refVec );
}

inline int pseudoInterfaceComparator( co::int32 key, PseudoInterface* itf )
{
	return ( itf->getKey() == key ? 0 : ( key < itf->getKey() ? -1 : 1 ) );
}

TEST( RefVectorTests, sortedRefVector )
{
	co::RefPtr<PseudoInterface> o1 = new PseudoInterface( 1 );
	co::RefPtr<PseudoInterface> o2 = new PseudoInterface( 2 );
	co::RefPtr<PseudoInterface> o3 = new PseudoInterface( 3 );
	co::RefPtr<PseudoInterface> o4 = new PseudoInterface( 4 );

	co::RefVector<PseudoInterface> refVec;
	refVec.sortedInsert( o2->getKey(), o2.get(), pseudoInterfaceComparator );
	refVec.sortedInsert( o4->getKey(), o4.get(), pseudoInterfaceComparator );
	refVec.sortedInsert( o3->getKey(), o3.get(), pseudoInterfaceComparator );
	refVec.sortedInsert( o1->getKey(), o1.get(), pseudoInterfaceComparator );

	EXPECT_EQ( refVec[0], o1 );
	EXPECT_EQ( refVec[1], o2 );
	EXPECT_EQ( refVec[2], o3 );
	EXPECT_EQ( refVec[3], o4 );

	// sanity check
	EXPECT_NE( refVec[0], refVec[3] );
	EXPECT_EQ( refVec.sortedFind( o3->getKey(), pseudoInterfaceComparator ), 2 );
}
