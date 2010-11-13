/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "Vec2D_Adapter.h"
#include <cmath>

double moduleA::Vec2D_Adapter::getX( moduleA::Vec2D& self )
{
	return self.first;
}

void moduleA::Vec2D_Adapter::setX( moduleA::Vec2D& self, double x )
{
	self.first = x;
}

double moduleA::Vec2D_Adapter::getY( moduleA::Vec2D& self )
{
	return self.second;
}

void moduleA::Vec2D_Adapter::setY( moduleA::Vec2D& self, double y )
{
	self.second = y;
}

double moduleA::Vec2D_Adapter::getLength( moduleA::Vec2D& self )
{
	return sqrt( getLength2( self ) );
}

double moduleA::Vec2D_Adapter::getLength2( moduleA::Vec2D& self )
{
	return self.first * self.first + self.second * self.second;
}

void moduleA::Vec2D_Adapter::set( moduleA::Vec2D& self, double x, double y )
{
	self.first = x;
	self.second = y;
}

void moduleA::Vec2D_Adapter::get( moduleA::Vec2D& self, double& x, double& y )
{
	x = self.first;
	y = self.second;
}

double moduleA::Vec2D_Adapter::dot( moduleA::Vec2D& self, const moduleA::Vec2D& other )
{
	return self.first * other.first + self.second * other.second;
}

bool moduleA::Vec2D_Adapter::equals( moduleA::Vec2D& self, const moduleA::Vec2D& other )
{
	return self.first == other.first && self.second == other.second;
}

const moduleA::Vec2D& moduleA::Vec2D_Adapter::testInOut( moduleA::Vec2D& self,
		const moduleA::Vec2D& a, moduleA::Vec2D& b, moduleA::Vec2D& c )
{
	c = b;
	b = a;
	return self;
}

co::Interface* moduleA::Vec2D_Adapter::dummyMethod( moduleA::Vec2D&, const co::Any& in, co::Any& out )
{
	out = in;
	return NULL;
}
