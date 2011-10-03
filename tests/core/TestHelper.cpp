/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"
#include <co/RefPtr.h>
#include <co/ISystem.h>
#include <co/INamespace.h>
#include <co/ITypeManager.h>
#include <core/utils/StringTokenizer.h>
#include <gtest/gtest.h>

co::ITypeBuilder* TestHelper::createBuilder( co::TypeKind kind, const std::string& fullTypeName, co::ITypeTransaction* tct )
{
	co::INamespace* ns = co::getSystem()->getTypes()->getRootNS();

	co::StringTokenizer st( fullTypeName, "." );
	st.nextToken();
	std::string currentToken = st.getToken();
	while( st.nextToken() )
	{
		co::INamespace* childNS = ns->getChildNamespace( currentToken );
		if( !childNS )
			childNS = ns->defineChildNamespace( currentToken );
		ns = childNS;
		currentToken = st.getToken();
	}

	co::ITypeBuilder* typeBuilder = ns->defineType( currentToken, kind, tct );
	EXPECT_TRUE( typeBuilder != NULL );

	return typeBuilder;
}

co::IType* TestHelper::type( const std::string& fullTypeName )
{
	return co::getSystem()->getTypes()->findType( fullTypeName );
}
