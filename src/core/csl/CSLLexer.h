/** \file
 *  This C header file was generated by $ANTLR version 3.3 Nov 30, 2010 12:45:30
 *
 *     -  From the grammar source file : /Users/tbastos/Projects/coral/src/core/csl/CSL.g
 *     -                            On : 2011-03-28 14:54:39
 *     -                 for the lexer : CSLLexerLexer *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer CSLLexer has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 * 
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pCSLLexer, which is returned from a call to CSLLexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 * * The methods in pCSLLexer are  as follows:
 *
 *  -  void      pCSLLexer->BASIC_TYPE(pCSLLexer)
 *  -  void      pCSLLexer->CLASS(pCSLLexer)
 *  -  void      pCSLLexer->CPP_SCOPE(pCSLLexer)
 *  -  void      pCSLLexer->COMMA(pCSLLexer)
 *  -  void      pCSLLexer->COMPONENT(pCSLLexer)
 *  -  void      pCSLLexer->ENUM(pCSLLexer)
 *  -  void      pCSLLexer->EXCEPTION_TOKEN(pCSLLexer)
 *  -  void      pCSLLexer->EXTENDS(pCSLLexer)
 *  -  void      pCSLLexer->IMPORT(pCSLLexer)
 *  -  void      pCSLLexer->INTERFACE(pCSLLexer)
 *  -  void      pCSLLexer->NATIVE(pCSLLexer)
 *  -  void      pCSLLexer->TOK_IN(pCSLLexer)
 *  -  void      pCSLLexer->TOK_INOUT(pCSLLexer)
 *  -  void      pCSLLexer->TOK_OUT(pCSLLexer)
 *  -  void      pCSLLexer->PROVIDES(pCSLLexer)
 *  -  void      pCSLLexer->RAISES(pCSLLexer)
 *  -  void      pCSLLexer->READONLY(pCSLLexer)
 *  -  void      pCSLLexer->RECEIVES(pCSLLexer)
 *  -  void      pCSLLexer->SEMICOLON(pCSLLexer)
 *  -  void      pCSLLexer->STRUCT(pCSLLexer)
 *  -  void      pCSLLexer->TOK_VOID(pCSLLexer)
 *  -  void      pCSLLexer->OPEN_BLOCK(pCSLLexer)
 *  -  void      pCSLLexer->OPEN_BRACE(pCSLLexer)
 *  -  void      pCSLLexer->OPEN_PAREN(pCSLLexer)
 *  -  void      pCSLLexer->CLOSE_BLOCK(pCSLLexer)
 *  -  void      pCSLLexer->CLOSE_BRACE(pCSLLexer)
 *  -  void      pCSLLexer->CLOSE_PAREN(pCSLLexer)
 *  -  void      pCSLLexer->DOT(pCSLLexer)
 *  -  void      pCSLLexer->ID(pCSLLexer)
 *  -  void      pCSLLexer->COMMENT(pCSLLexer)
 *  -  void      pCSLLexer->CPP_BLOCK(pCSLLexer)
 *  -  void      pCSLLexer->CPP_TAG(pCSLLexer)
 *  -  void      pCSLLexer->WS(pCSLLexer)
 *  -  void      pCSLLexer->Tokens(pCSLLexer)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD licence"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_CSLLexer_H
#define _CSLLexer_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */
 
#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct CSLLexer_Ctx_struct CSLLexer, * pCSLLexer;



	#define	ANTLR3_INLINE_INPUT_8BIT


#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule 
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/** Context tracking structure for CSLLexer
 */
struct CSLLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;


     void (*mBASIC_TYPE)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mCLASS)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mCPP_SCOPE)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mCOMMA)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mCOMPONENT)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mENUM)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mEXCEPTION_TOKEN)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mEXTENDS)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mIMPORT)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mINTERFACE)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mNATIVE)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mTOK_IN)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mTOK_INOUT)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mTOK_OUT)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mPROVIDES)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mRAISES)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mREADONLY)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mRECEIVES)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mSEMICOLON)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mSTRUCT)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mTOK_VOID)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mOPEN_BLOCK)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mOPEN_BRACE)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mOPEN_PAREN)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mCLOSE_BLOCK)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mCLOSE_BRACE)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mCLOSE_PAREN)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mDOT)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mID)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mCOMMENT)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mCPP_BLOCK)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mCPP_TAG)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mWS)	(struct CSLLexer_Ctx_struct * ctx);
     void (*mTokens)	(struct CSLLexer_Ctx_struct * ctx);
    const char * (*getGrammarFileName)();
    void            (*reset)  (struct CSLLexer_Ctx_struct * ctx);
    void	    (*free)   (struct CSLLexer_Ctx_struct * ctx);
        
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pCSLLexer CSLLexerNew         (pANTLR3_INPUT_STREAM instream);
ANTLR3_API pCSLLexer CSLLexerNewSSD      (pANTLR3_INPUT_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the lexer will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif 
#define EOF      -1
#define IMPORT      4
#define SEMICOLON      5
#define EXCEPTION_TOKEN      6
#define ID      7
#define ENUM      8
#define OPEN_BLOCK      9
#define CLOSE_BLOCK      10
#define STRUCT      11
#define NATIVE      12
#define CLASS      13
#define OPEN_PAREN      14
#define CPP_TAG      15
#define CLOSE_PAREN      16
#define INTERFACE      17
#define COMPONENT      18
#define DOT      19
#define CPP_SCOPE      20
#define COMMA      21
#define READONLY      22
#define RAISES      23
#define CPP_BLOCK      24
#define EXTENDS      25
#define TOK_VOID      26
#define BASIC_TYPE      27
#define OPEN_BRACE      28
#define CLOSE_BRACE      29
#define TOK_IN      30
#define TOK_OUT      31
#define TOK_INOUT      32
#define PROVIDES      33
#define RECEIVES      34
#define COMMENT      35
#define WS      36
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for CSLLexer
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
