local function template( writer, c, ... )
	if c.moduleName == 'co' or c.moduleName == 'lua' then
		writer( [[
/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */
]] )
	else
		writer( "/*******************************************************************************\n** " )
		writer( ... )
		writer( [[

**
** Created: ]], c.utils.currentDateTime(), '\n', [[
**      by: Coral Compiler version ]], co.version, '\n', [[
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/
]] )
	end


end

return template
