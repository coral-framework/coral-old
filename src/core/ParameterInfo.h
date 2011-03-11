/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _PARAMETERINFO_H_
#define _PARAMETERINFO_H_

#include "ParameterInfoComponent_Base.h"
#include <co/RefPtr.h>
#include <co/Platform.h>

namespace co {

/*!
	Component that implements co.ParameterInfo.
 */
class ParameterInfoComponent : public ParameterInfoComponent_Base
{
public:
	ParameterInfoComponent();
	virtual ~ParameterInfoComponent();

	// internal methods:
	void init( const std::string& name, Type* type, bool isIn, bool isOut );
	void setName( const std::string& name );
	void setType( Type* type );
	void setIsIn( bool isIn );
	void setIsOut( bool isOut );

	// ParameterInfo methods
	const std::string& getName();
	Type* getType();
	bool getIsIn();
	bool getIsOut();

private:
	std::string _name;
	Type* _type;
	bool _isIn;
	bool _isOut;
};

} // namespace co

#endif
