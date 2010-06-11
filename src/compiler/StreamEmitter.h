/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _STREAMEMITTER_H_
#define _STREAMEMITTER_H_

#include <ctemplate/template_emitter.h>
#include <fstream>

namespace ctemplate {
	class Template;
	class TemplateDictionaryInterface;
}

/*!
	A specialization of ctemplate::ExpandEmitter that expands templates into std::ostream's.
 */
class StreamEmitter : public ctemplate::ExpandEmitter
{
public:
	StreamEmitter( std::ostream& ostream );

	virtual ~StreamEmitter();
	
	//! Utility function to expand a template into this stream. 
	void expand( ctemplate::Template* tpl, const ctemplate::TemplateDictionaryInterface* data );
	
	//! Utility function to get a template by name and expand it into a stream.
	//! Throws an exception if the template cannot be loaded.
	void expand( const char* templateName, const ctemplate::TemplateDictionaryInterface* data );

	// ctemplate::ExpandEmitter Methods:
	virtual void Emit( char c );
	virtual void Emit( const std::string& s );
	virtual void Emit( const char* s );
	virtual void Emit( const char* s, size_t slen );

private:
	std::ostream& _ostream;
};

/*!
	A specialization of StreamEmitter that expands templates directly into files.
	This class also handles the creation of directories, if needed to create the output file.
 */
class FileStreamEmitter : public StreamEmitter
{
public:
	FileStreamEmitter();
	FileStreamEmitter( const std::string& dir, const std::string& filename );

	virtual ~FileStreamEmitter();

	void reopen( const std::string& dir, const std::string& filename );
	
private:
	std::ofstream _ofstream;
};

#endif // _STREAMEMITTER_H_
