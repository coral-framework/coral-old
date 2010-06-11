/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/RefVector.h>
#include <co/Interface.h>
#include <co/Namespace.h>
#include <co/UserException.h>

#include <gtest/gtest.h>

class PseudoInterface : public co::Interface
{
public:
	PseudoInterface( const char* name = "",  bool* setToTrueWhenDestroyed = 0 ) :
			_name( name ), _setToTrueWhenDestroyed( setToTrueWhenDestroyed ), _refCount( 0 )
	{;}

	virtual ~PseudoInterface()
	{
		if( _setToTrueWhenDestroyed )
			*_setToTrueWhenDestroyed = true;
	}

	virtual co::InterfaceType* getInterfaceType() { return 0; }
	virtual co::Component* getInterfaceOwner() { return 0; }
	virtual const std::string& getInterfaceName() { return _name; }
	virtual void componentRetain() { ++_refCount; }
	virtual void componentRelease() { if( --_refCount <= 0 ) delete this; }

private:
	std::string _name;
	bool* _setToTrueWhenDestroyed;
	co::int32 _refCount;
};

TEST( RefVectorTests, rawPtrVectorEquivalence )
{
	PseudoInterface o1, o2, o3;
	o1.componentRetain();
	o2.componentRetain();
	o3.componentRetain();
	
	co::RefPtr<PseudoInterface> refPtr( &o1 );
	EXPECT_EQ( sizeof( co::RefPtr<PseudoInterface> ), sizeof( PseudoInterface* ) );

	PseudoInterface* rawVec[] = { &o1, &o2, &o3 };

	co::RefVector<PseudoInterface> refVec;
	refVec.push_back( &o1 );
	refVec.push_back( &o2 );
	refVec.push_back( &o3 );

	co::ArrayRange<PseudoInterface* const> range( refVec );
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

	co::ArrayRange<PseudoInterface* const> range( emptyRefVec );
	EXPECT_TRUE( range.isEmpty() );
}

TEST( RefVectorTests, castedPtrRange )
{
	PseudoInterface o1, o2, o3;
	o1.componentRetain();
	o2.componentRetain();
	o3.componentRetain();

	co::RefVector<PseudoInterface> refVec;
	refVec.push_back( &o1 );
	refVec.push_back( &o2 );
	refVec.push_back( &o3 );

	co::ArrayRange<co::Interface* const> range( refVec );
	EXPECT_EQ( range.getSize(), 3 );
	
	// invalid usage sample (causes compile-time error):
	//co::ArrayRange<co::Namespace*> invalidRange( refVec );
}

inline int pseudoInterfaceComparator( PseudoInterface* element, const std::string& name )
{
	return element->getInterfaceName().compare( name );
}

TEST( RefVectorTests, sortedRefVector )
{
	co::RefPtr<PseudoInterface> o1 = new PseudoInterface( "o1" );
	co::RefPtr<PseudoInterface> o2 = new PseudoInterface( "o2" );
	co::RefPtr<PseudoInterface> o3 = new PseudoInterface( "o3" );
	co::RefPtr<PseudoInterface> o4 = new PseudoInterface( "o4" );

	co::RefVector<PseudoInterface> refVec;
	refVec.sortedInsert( o2->getInterfaceName(), o2, pseudoInterfaceComparator );
	refVec.sortedInsert( o4->getInterfaceName(), o4, pseudoInterfaceComparator );
	refVec.sortedInsert( o3->getInterfaceName(), o3.get(), pseudoInterfaceComparator );
	refVec.sortedInsert( o1->getInterfaceName(), o1, pseudoInterfaceComparator );

	EXPECT_EQ( refVec[0], o1 );
	EXPECT_EQ( refVec[1], o2 );
	EXPECT_EQ( refVec[2], o3 );
	EXPECT_EQ( refVec[3], o4 );

	// sanity check
	EXPECT_NE( refVec[0], refVec[3] );

	int pos;
	EXPECT_TRUE( refVec.sortedFind( std::string( "o3" ), pseudoInterfaceComparator, pos ) );
	EXPECT_EQ( pos, 2 );
}
