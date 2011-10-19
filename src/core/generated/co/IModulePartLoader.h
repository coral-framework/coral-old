/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IMODULEPARTLOADER_H_
#define _CO_IMODULEPARTLOADER_H_

#include <co/TypeTraits.h>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IModulePart;
} // namespace co
// End Of Forward Declarations

// co.IModulePartLoader Mapping:
namespace co {

class IModulePartLoader : public co::IService
{
public:
	virtual ~IModulePartLoader() {;}

	virtual bool canLoadModulePart( const std::string& moduleName ) = 0;

	virtual co::IModulePart* loadModulePart( const std::string& moduleName ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IModulePartLoader> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IModulePartLoader> { static const char* get() { return "co.IModulePartLoader"; } };
} // namespace co

#endif // _CO_IMODULEPARTLOADER_H_
