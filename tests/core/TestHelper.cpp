/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"
#include <co/RefPtr.h>
#include <co/System.h>
#include <co/Namespace.h>
#include <co/TypeManager.h>
#include <core/tools/StringTokenizer.h>
#include <gtest/gtest.h>

co::TypeBuilder* TestHelper::createBuilder( co::TypeKind kind, const std::string& fullTypeName, co::TypeCreationTransaction* tct )
{
	co::Namespace* ns = co::getSystem()->getTypes()->getRootNS();

	co::StringTokenizer st( fullTypeName, "." );
	st.nextToken();
	std::string currentToken = st.getToken();
	while( st.nextToken() )
	{
		co::Namespace* childNS = ns->getChildNamespace( currentToken );
		if( !childNS )
			childNS = ns->defineChildNamespace( currentToken );
		ns = childNS;
		currentToken = st.getToken();
	}

	co::TypeBuilder* typeBuilder = ns->defineType( currentToken, kind, tct );
	EXPECT_TRUE( typeBuilder != NULL );

	return typeBuilder;
}

co::Type* TestHelper::type( const std::string& fullTypeName )
{
	return co::getSystem()->getTypes()->findType( fullTypeName );
}
