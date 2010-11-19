/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _MODULEPART_H_
#define _MODULEPART_H_

#include "moduleB_Base.h"

class ModulePart : public moduleB::moduleB_Base
{
public:
    ModulePart();
	virtual ~ModulePart();

	// co::ModulePart methods:
	void initialize( co::Module* module );
	void integrate( co::Module* module );
	void integratePresentation( co::Module* module );
	void disintegrate( co::Module* module );
	void dispose( co::Module* module );
};

#endif
