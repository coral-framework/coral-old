/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_CSL_ERROR_H_
#define _CO_CSL_ERROR_H_

#include "location.hh"
#include "../tools/RefCounted.h"

namespace co {
namespace csl {

/*!
	Represents an error stack, with information about the errors that occurred
	while attempting to load a CSL file.

	The error line and filename may not be available in some cases, such as "file not found" and
	semantic errors. (In this case, the filename will be empty and the line will be -1.)
 */
class CORAL_EXPORT Error : public RefCounted
{
public:
	Error( const location& loc, const std::string& message, Error* innerError );
	Error( const std::string& filename, const std::string& message, Error* innerError );
	Error(const std::string& message, Error* innerError );

	virtual ~Error();

	inline const std::string& getMessage() const { return _message; }

	inline const std::string& getFileName() const { return _filename; }

	inline int32 getLine() const { return _line; }

	inline int32 getColumn() const { return _column; }

	inline const Error* getInnerError() const { return _innerError.get(); }

private:
	std::string _message;
	std::string _filename;
	int32 _line;
	int32 _column;
	RefPtr<Error> _innerError;
};

} // namespace csl
} // namespace co

/*!
	Prints a CSL error stack. The message is formatted as follows:

	a) When errors have filenames and lines, the message is formatted like an error stack:
	\verbatim
		From file: /home/user/mymodules/MyCSLType.csl:6: cannot load super-type 'mymodules.MyParentType':
			From file: /home/user/mymodules/MyParentType.csl:2: cannot load dependent type 'tools.ToolType':
				In file: /home/user/mymodules/tools/ToolType.csl:9: syntax error near '@type' : expected identifier.
	\endverbatim

	b) In the case of semantic errors, lines and filenames are not available, and since these errors
		are never nested, the formatted message is just a plain error message.
 */
CORAL_EXPORT std::ostream& operator<<( std::ostream& out, const co::csl::Error& error );

#endif // _CO_CSL_ERROR_H_
