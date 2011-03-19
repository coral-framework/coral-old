/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _PARAMETERINFO_H_
#define _PARAMETERINFO_H_

#include "ParameterInfo_Base.h"
#include <co/RefPtr.h>
#include <co/Platform.h>

namespace co {

/*!
	Implements co.IParameterInfo.
 */
class ParameterInfo : public ParameterInfo_Base
{
public:
	ParameterInfo();
	virtual ~ParameterInfo();

	// internal methods:
	void init( const std::string& name, IType* type, bool isIn, bool isOut );
	void setName( const std::string& name );
	void setType( IType* type );
	void setIsIn( bool isIn );
	void setIsOut( bool isOut );

	// IParameterInfo methods
	const std::string& getName();
	IType* getType();
	bool getIsIn();
	bool getIsOut();

private:
	std::string _name;
	IType* _type;
	bool _isIn;
	bool _isOut;
};

} // namespace co

#endif
