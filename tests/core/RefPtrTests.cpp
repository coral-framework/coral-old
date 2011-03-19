/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/RefVector.h>
#include <co/Interface.h>
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
	
	inline co::int32 getRefCount() const { return _refCount; }

	virtual co::IInterfaceType* getInterfaceType() { return 0; }
	virtual co::IComponent* getInterfaceOwner() { return 0; }
	virtual const std::string& getInterfaceName() { return _name; }
	virtual void componentRetain() { ++_refCount; }
	virtual void componentRelease() { if( --_refCount <= 0 ) delete this; }

private:
	std::string _name;
	bool* _setToTrueWhenDestroyed;
	co::int32 _refCount;
};

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
	PseudoInterface *obj = new PseudoInterface( "", &objDestroyed );
	{
		obj->componentRetain();
		co::RefPtr<PseudoInterface> ptr = obj;
	}

	ASSERT_FALSE( objDestroyed );
	obj->componentRelease();
	EXPECT_TRUE( objDestroyed );

	bool anonymousInstanceDestroyed = false;
	{
		co::RefPtr<PseudoInterface>( new PseudoInterface( "", &anonymousInstanceDestroyed ) );
	}
	EXPECT_TRUE( anonymousInstanceDestroyed );

}

TEST( RefPtrTests, refPtrSwap )
{
	bool objDestroyed = false;
	PseudoInterface *obj = new PseudoInterface( "", &objDestroyed );

	co::RefPtr<PseudoInterface> ptr1;
	{
		co::RefPtr<PseudoInterface> ptr2( obj );
		ptr2.swap( ptr1 );
		ptr1.swap( ptr2 );
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
	PseudoInterface *objA = new PseudoInterface( "A", &destroyedA );

	bool destroyedB = false;
	PseudoInterface *objB = new PseudoInterface( "B", &destroyedB );

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
