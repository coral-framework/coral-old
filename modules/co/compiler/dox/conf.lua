local function template( writer, c )
	writer( [[
# For good results you must use the Doxygen settings below. Either edit
# your Doxyfile accordingly, or append this file when invoking Doxygen;
# e.g.: ( cat Doxyfile ; cat ./generated/Doxygen.conf ) | doxygen -
CPP_CLI_SUPPORT			= YES
IDL_PROPERTY_SUPPORT	= YES
SHOW_USED_FILES			= NO
FULL_PATH_NAMES			= NO
TAB_SIZE				= 4
]] )
end

return template
