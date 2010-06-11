/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "Uuid_Adapter.h"
#include "tools/CryptoHash.h"
	
bool co::Uuid_Adapter::getIsNull( const co::Uuid& instance )
{
	return instance.isNull();
}

void co::Uuid_Adapter::clear( co::Uuid& instance )
{
	instance.clear();
}

void co::Uuid_Adapter::createRandom( co::Uuid& instance )
{
	instance = co::Uuid::createRandom();
}

void co::Uuid_Adapter::createSha1( co::Uuid& instance, const std::string& data )
{
	co::CryptoHash hash( co::CryptoHash::Sha1 );
	hash.addData( data );

	co::CryptoHash::Result result;
	hash.getResult( result );

	result.toUuid( instance );
}

void co::Uuid_Adapter::getString( co::Uuid& instance, std::string& str )
{
	instance.getString( str );
}

void co::Uuid_Adapter::setString( co::Uuid& instance, const std::string& str )
{
	instance.setString( str );
}
