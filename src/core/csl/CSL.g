grammar CSL;

options {
	backtrack = false;
	language = C;
	k = 1;
}

@parser::includes
{
	#define pANTLR3_PARSER	csl::Parser* parser; pANTLR3_PARSER
	namespace csl {
		class Parser;
	}
}

@parser::postinclude
{
	#undef pANTLR3_PARSER
	#include "Parser.h"
	#include <co/TypeKind.h>
}

/*****************************************************************************
 * Parser Rules
 *****************************************************************************/

csl_file
	: specification+ EOF
	;

specification
	: comment
	| import_clause
	| enum_specification
	| exception_specification
	| struct_specification
	| native_class_specification
	| interface_specification
	| component_specification
	;

import_clause
	: IMPORT { ctx->parser->setCurrentLine( $IMPORT.line ); } qualified_identifier SEMICOLON
		{
			ctx->parser->onImportClause( (const char*)$qualified_identifier.text->chars );
		}
	;

exception_specification
	: EXCEPTION_TOKEN { ctx->parser->setCurrentLine( $EXCEPTION_TOKEN.line ); } ID SEMICOLON
		{
			ctx->parser->onTypeSpecification( (const char*)$ID.text->chars, co::TK_EXCEPTION );
		}
	;

enum_specification
	: ENUM { ctx->parser->setCurrentLine( $ENUM.line ); } ID
		{
			ctx->parser->onTypeSpecification( (const char*)$ID.text->chars, co::TK_ENUM );
		}
		OPEN_BLOCK identifier_list CLOSE_BLOCK SEMICOLON
	;

struct_specification
	: STRUCT { ctx->parser->setCurrentLine( $STRUCT.line ); } ID
		{
			ctx->parser->onTypeSpecification( (const char*)$ID.text->chars, co::TK_STRUCT );
		}
		OPEN_BLOCK struct_member* CLOSE_BLOCK SEMICOLON
	;

native_class_specification
	: NATIVE { ctx->parser->setCurrentLine( $NATIVE.line ); } CLASS ID
		{
			ctx->parser->onTypeSpecification( (const char*)$ID.text->chars, co::TK_NATIVECLASS );
		}
		OPEN_PAREN CPP_TAG cpp_type CLOSE_PAREN
		{
			ctx->parser->onNativeClass( (const char*)$CPP_TAG.text->chars, (const char*)$cpp_type.text->chars );
		}
		OPEN_BLOCK native_class_member* CLOSE_BLOCK SEMICOLON
	;

identifier_list
	: comment* firstId = ID
		{
			ctx->parser->setCurrentLine( $firstId.line );
			ctx->parser->onIdentifierListItem( (const char*)$firstId.text->chars );
		}
		( COMMA comment* loopId = ID
			{
				ctx->parser->setCurrentLine( $loopId.line );
				ctx->parser->onIdentifierListItem( (const char*)$loopId.text->chars );
			}
		)* comment*
	;

interface_specification
	: INTERFACE { ctx->parser->setCurrentLine( $INTERFACE.line ); } ID
		{
			ctx->parser->onTypeSpecification( (const char*)$ID.text->chars, co::TK_INTERFACE );
		}
		inheritance_declaration? OPEN_BLOCK interface_member* CLOSE_BLOCK SEMICOLON
	;

component_specification
	: COMPONENT { ctx->parser->setCurrentLine( $COMPONENT.line ); } ID
		{
			ctx->parser->onTypeSpecification( (const char*)$ID.text->chars, co::TK_COMPONENT  );
		}
		OPEN_BLOCK component_member* CLOSE_BLOCK SEMICOLON
	;

qualified_identifier
	: ID ( DOT ID )*
	;

cpp_type
	: ID ( CPP_SCOPE ID )* CPP_TAG?
	;

native_class_member
	: comment
	| attribute_declaration
	| method_declaration
	;

struct_member
	: comment
	| { ctx->parser->setCurrentLine( $start->line ); } type_declaration ID SEMICOLON
		{
			ctx->parser->onStructMember( (const char*)$ID.text->chars );
		}
	;

interface_member
	: comment
	| attribute_declaration
	| method_declaration
	| CPP_BLOCK { ctx->parser->onCppBlock( (const char*)$CPP_BLOCK.text->chars ); }
	;

inheritance_declaration
	: COLON firstInheritance = qualified_identifier
		{
			ctx->parser->onSuperType( (const char*)$firstInheritance.text->chars );
		}
		( COMMA loopInheritance = qualified_identifier { ctx->parser->onSuperType( (const char*)$loopInheritance.text->chars ); } )*
	;

attribute_declaration
	: READONLY? ATTRIBUTE { ctx->parser->setCurrentLine( $start->line ); } type_declaration ID SEMICOLON
		{
			bool isReadOnly = false;
			if( $READONLY )
				isReadOnly = true;
			ctx->parser->onAttribute( isReadOnly, (const char*)$ID.text->chars );
		}
	;

method_declaration
	: { ctx->parser->setCurrentLine( $start->line ); } method_type_spec ID OPEN_PAREN
		{
			ctx->parser->onMethod( (char*)$ID.text->chars );
		}
		parameter_declaration_list? CLOSE_PAREN
		exception_list? SEMICOLON { ctx->parser->onEndMethod(); }
	;

parameter_declaration_list
	: parameter_declaration ( COMMA parameter_declaration )*
	;

parameter_declaration
	: param_io type_declaration ID
		{
			ctx->parser->onParameter( $param_io.isIn, $param_io.isOut, (const char*)$ID.text->chars );
		}
	;

exception_list
	: RAISES { ctx->parser->setCurrentLine( $RAISES.line ); } firstExcep=qualified_identifier
		{
			ctx->parser->onExeptionRaised( (const char*)$firstExcep.text->chars );
		}
		( COMMA loopExcep=qualified_identifier { ctx->parser->onExeptionRaised( (const char*)$loopExcep.text->chars ); } )*
	;

component_member
	: comment
	| { ctx->parser->setCurrentLine( $start->line ); } interface_role qualified_identifier
		{
			ctx->parser->onTypeDeclaration( (const char*)$qualified_identifier.text->chars, false );
		}
		ID SEMICOLON
		{
			ctx->parser->onComponentInterface( $interface_role.isFacet, (const char*)$ID.text->chars );
		}
	;

method_type_spec
	: TOK_VOID
		{
			ctx->parser->onTypeDeclaration( "void", false );
		}
	| type_declaration
	;

type_declaration
	@declarations { bool isArray; const char* qualifiedName; }
	@init { isArray = false; qualifiedName = NULL; }
	:
		(
			BASIC_TYPE { qualifiedName = (const char*)$BASIC_TYPE.text->chars; }
			| qualified_identifier { qualifiedName = (const char*)$qualified_identifier.text->chars; }
		)
		( pair_of_braces { isArray = true; } ) ?
  		{
  			ctx->parser->onTypeDeclaration( qualifiedName, isArray );
  		}
  	;

pair_of_braces
	: OPEN_BRACE CLOSE_BRACE
	;

param_io returns [ bool isIn, bool isOut ]
	@init { $isIn = false; $isOut = false; }
	: TOK_IN{ $isIn = true; }
	| TOK_OUT{ $isOut = true; }
	| TOK_INOUT{ $isIn = true; $isOut = true; }
	;

interface_role returns [ bool isFacet ]
	: PROVIDES { $isFacet = true; }
	| RECEIVES { $isFacet = false; }
	;

comment
	: COMMENT { ctx->parser->onComment( (const char*)$COMMENT.text->chars ); }
	;

/*****************************************************************************
 * Lexer Rules
 *****************************************************************************/

ATTRIBUTE
	: 'attribute'
	;

BASIC_TYPE
	: 'any'
	| 'bool'
	| 'int8'
	| 'uint8'
	| 'int16'
	| 'uint16'
	| 'int32'
	| 'uint32'
	| 'int64'
	| 'uint64'
	| 'float'
	| 'double'
	| 'string'
	;

CLASS
	: 'class'
	;

CPP_SCOPE
	: '::'
	;

COLON
	: ':'
	;

COMMA
	: ','
	;

COMPONENT
	: 'component'
	;

ENUM
	: 'enum'
	;

EXCEPTION_TOKEN
	: 'exception'
	;

IMPORT
	: 'import'
	;

INTERFACE
	: 'interface'
	;

NATIVE
	: 'native'
	;

TOK_IN
	: 'in'
	;

TOK_INOUT
	: 'inout'
	;

TOK_OUT
	: 'out'
	;

PROVIDES
	: 'provides'
	;

RAISES
	: 'raises'
	;

READONLY
	: 'readonly'
	;

RECEIVES
	: 'receives'
	;

SEMICOLON
	: ';'
	;

STRUCT
	: 'struct'
	;

TOK_VOID
	: 'void'
	;

OPEN_BLOCK
	: '{'
	;

OPEN_BRACE
	: '['
	;

OPEN_PAREN
	: '('
	;

CLOSE_BLOCK
	: '}'
	;

CLOSE_BRACE
	: ']'
	;

CLOSE_PAREN
	: ')'
	;

DOT	: '.' ;

ID	: ('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
	;

COMMENT
	: '//' ~('\n'|'\r')*  '\r'?  '\n'
	| '/*' ( options { greedy = false; } : . )* '*/'
	;

CPP_BLOCK
	: '<c++' ( options { greedy = false; } : . )* 'c++>'
	;

CPP_TAG
	: '<' ('a'..'z'|'A'..'Z'|'0'..'9'|'_'|':'|'.'|','|'/'|' ')* '>'
	;

WS
	:  (' '|'\t'|'\r'|'\n') { $channel = HIDDEN; }
    ;
