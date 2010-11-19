/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _PARAMETERINFO_H_
#define _PARAMETERINFO_H_

#include "ParameterInfoComponent_Base.h"
#include <co/RefPtr.h>
#include <co/Platform.h>

/*!
	Component that implements co.ParameterInfo.
 */
class ParameterInfo : public co::ParameterInfoComponent_Base
{
public:
	ParameterInfo();
	virtual ~ParameterInfo();

	// internal methods:
	void init( const std::string& name, co::Type* type, bool isIn, bool isOut );
	void setName( const std::string& name );
	void setType( co::Type* type );
	void setIsIn( bool isIn );
	void setIsOut( bool isOut );

	// co::ParameterInfo methods
	const std::string& getName();
	co::Type* getType();
	bool getIsIn();
	bool getIsOut();

private:
	std::string _name;
	co::Type* _type;
	bool _isIn;
	bool _isOut;
};

#endif
