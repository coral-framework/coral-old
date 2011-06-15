/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/RefVector.h>
#include <co/IService.h>
#include <gtest/gtest.h>

namespace {

class PseudoInterface : public co::IService
{
public:
	PseudoInterface( bool* setToTrueWhenDestroyed = 0 ) :
		_setToTrueWhenDestroyed( setToTrueWhenDestroyed ), _refCount( 0 )
	{;}

	virtual ~PseudoInterface()
	{
		if( _setToTrueWhenDestroyed )
			*_setToTrueWhenDestroyed = true;
	}

	inline co::int32 getRefCount() const { return _refCount; }

	co::IInterface* getInterface() { return 0; }
	co::IObject* getProvider() { return 0; }
	co::IPort* getFacet() { return 0; }
	void serviceRetain() { ++_refCount; }
	void serviceRelease() { if( --_refCount <= 0 ) delete this; }

private:
	bool* _setToTrueWhenDestroyed;
	co::int32 _refCount;
};

} // anonymous namespace

TEST( RefPtrTests, refPtrValidity )
{
	co::RefPtr<PseudoInterface> ptr;
	EXPECT_TRUE( !ptr );
	EXPECT_FALSE( ptr.isValid() );

	ptr = new PseudoInterface;
	EXPECT_FALSE( !ptr );
	EXPECT_TRUE( ptr.isValid() );

	ptr = NULL;
	EXPECT_TRUE( !ptr );
	EXPECT_FALSE( ptr.isValid() );
}

TEST( RefPtrTests, refPtrRelease )
{
	bool objDestroyed = false;
	PseudoInterface *obj = new PseudoInterface( &objDestroyed );
	{
		obj->serviceRetain();
		co::RefPtr<PseudoInterface> ptr = obj;
	}

	ASSERT_FALSE( objDestroyed );
	obj->serviceRelease();
	EXPECT_TRUE( objDestroyed );

	bool anonymousInstanceDestroyed = false;
	{
		co::RefPtr<PseudoInterface>( new PseudoInterface( &anonymousInstanceDestroyed ) );
	}
	EXPECT_TRUE( anonymousInstanceDestroyed );

}

TEST( RefPtrTests, refPtrSwap )
{
	bool objDestroyed = false;
	PseudoInterface *obj = new PseudoInterface( &objDestroyed );

	co::RefPtr<PseudoInterface> ptr1;
	{
		using std::swap;
		co::RefPtr<PseudoInterface> ptr2( obj );
		ptr2.swap( ptr1 );
		swap( ptr1, ptr2 );
		ptr2.swap( ptr1 );
	}

	ASSERT_FALSE( objDestroyed );
	ptr1 = 0;
	EXPECT_TRUE( objDestroyed );
}

struct TripleRefs
{
	co::RefPtr<PseudoInterface> ref1;
	co::RefPtr<PseudoInterface> ref2;
	co::RefPtr<PseudoInterface> ref3;
};

TEST( RefPtrTests, refPtrsInStruct )
{
	bool destroyedA = false;
	PseudoInterface *objA = new PseudoInterface( &destroyedA );

	bool destroyedB = false;
	PseudoInterface *objB = new PseudoInterface( &destroyedB );

	TripleRefs structOne;

	EXPECT_EQ( objA->getRefCount(), 0 );
	EXPECT_EQ( objB->getRefCount(), 0 );

	structOne.ref1 = objA;
	structOne.ref2 = objA;
	structOne.ref3 = objB;

	EXPECT_EQ( objA->getRefCount(), 2 );
	EXPECT_EQ( objB->getRefCount(), 1 );

	TripleRefs structTwo;
	structTwo = structOne;

	structTwo.ref2 = objB;

	EXPECT_EQ( objA->getRefCount(), 3 );
	EXPECT_EQ( objB->getRefCount(), 3 );

	TripleRefs structThree( structOne );
	structOne = structTwo;

	EXPECT_EQ( objA->getRefCount(), 4 );
	EXPECT_EQ( objB->getRefCount(), 5 );

	TripleRefs emptyStruct;
	structThree = emptyStruct;

	EXPECT_EQ( objA->getRefCount(), 2 );
	EXPECT_EQ( objB->getRefCount(), 4 );
}
