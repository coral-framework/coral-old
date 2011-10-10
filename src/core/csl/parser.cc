/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

// Take the name prefix into account.
#define yylex   csl_lex

/* First part of user declarations.  */




#include "parser.hh"

/* User implementation prologue.  */


/* Unqualified %code blocks.  */


	#include "scanner.hh"
	#include <co/Exception.h>

	#define PARSE_EV( ev ) loader.ev; if( loader.getError() ) YYABORT;

	// declare the scanner function
	YY_DECL;




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                               \
 do                                                                    \
   if (N)                                                              \
     {                                                                 \
       (Current).begin = YYRHSLOC (Rhs, 1).begin;                      \
       (Current).end   = YYRHSLOC (Rhs, N).end;                        \
     }                                                                 \
   else                                                                \
     {                                                                 \
       (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;        \
     }                                                                 \
 while (false)
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace co { namespace csl {


  /// Build a parser object.
  Parser::Parser (co::csl::Loader& loader_yyarg, void* scanner_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      loader (loader_yyarg),
      scanner (scanner_yyarg)
  {
  }

  Parser::~Parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  Parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  Parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  Parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    
{
	yylloc.begin.filename = yylloc.end.filename = loader.getCurrentFileName();
}


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc, loader, scanner);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yy_table_value_is_error_ (yyn))
	  goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 3:

    {
			if( loader.getError() )
			{
				// we must've got a lexical error, so abort...
			}
			else
			{
				PUSH_ERROR( yylloc, "syntax error near '" << csl_get_text( scanner ) << "'" )
			}
			YYABORT;
		}
    break;

  case 16:

    { PARSE_EV( onImport( (yylocation_stack_[(2) - (1)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 17:

    { PARSE_EV( onComment( (yylocation_stack_[(1) - (1)]), *(yysemantic_stack_[(1) - (1)].str) ) ) }
    break;

  case 22:

    { (yyval.str) = (yysemantic_stack_[(1) - (1)].str); }
    break;

  case 23:

    { static std::string sComponent( "component" ); (yyval.str) = &sComponent; }
    break;

  case 24:

    { static std::string sInterface( "interface" ); (yyval.str) = &sInterface; }
    break;

  case 27:

    { PARSE_EV( onAnnotation( (yylocation_stack_[(1) - (1)]), *(yysemantic_stack_[(1) - (1)].str) ) ) }
    break;

  case 31:

    { PARSE_EV( onAnnotationData( (yylocation_stack_[(1) - (1)]), "value", *(yysemantic_stack_[(1) - (1)].any) ) ) }
    break;

  case 38:

    { PARSE_EV( onAnnotationData( (yyloc), *(yysemantic_stack_[(3) - (1)].str), *(yysemantic_stack_[(3) - (3)].any) ) ) }
    break;

  case 39:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), co::TK_ENUM, (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 41:

    { PARSE_EV( onEnumIdentifier( (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 45:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), co::TK_EXCEPTION, (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 46:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), co::TK_STRUCT, (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 54:

    { PARSE_EV( onField( (yylocation_stack_[(3) - (2)]), *(yysemantic_stack_[(3) - (2)].str), false ) ) }
    break;

  case 55:

    { PARSE_EV( onField( (yylocation_stack_[(4) - (3)]), *(yysemantic_stack_[(4) - (3)].str), true ) ) }
    break;

  case 56:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), co::TK_NATIVECLASS, (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 57:

    { PARSE_EV( onNativeClass( (yylocation_stack_[(7) - (4)]), *(yysemantic_stack_[(7) - (5)].str), *(yysemantic_stack_[(7) - (6)].str) ) ) }
    break;

  case 59:

    { (yyval.str) = (yysemantic_stack_[(1) - (1)].str); }
    break;

  case 60:

    { (yyval.str) = (yysemantic_stack_[(1) - (1)].str); }
    break;

  case 68:

    { PARSE_EV( onTypeDecl( (yylocation_stack_[(1) - (1)]), "void", false ) ) }
    break;

  case 70:

    { PARSE_EV( onMethod( (yylocation_stack_[(1) - (0)]), *(yysemantic_stack_[(1) - (0)].str) ) ) }
    break;

  case 71:

    { PARSE_EV( onMethodEnd( (yylocation_stack_[(6) - (0)])) ) }
    break;

  case 76:

    { int v = (yysemantic_stack_[(3) - (1)].i32);
		PARSE_EV( onParameter( (yylocation_stack_[(3) - (3)]), ( v & 1 ) != 0, ( v & 2 ) != 0, *(yysemantic_stack_[(3) - (3)].str) ) ) }
    break;

  case 77:

    { (yyval.i32) = 1; }
    break;

  case 78:

    { (yyval.i32) = 2; }
    break;

  case 79:

    { (yyval.i32) = 3; }
    break;

  case 80:

    { PUSH_ERROR( (yyloc), "expected 'in', 'out' or 'inout' before '"
		<< csl_get_text( scanner ) << "'" ); YYABORT; }
    break;

  case 85:

    { PARSE_EV( onRaises( (yylocation_stack_[(1) - (1)]), *(yysemantic_stack_[(1) - (1)].str) ) ) }
    break;

  case 86:

    { PARSE_EV( onTypeDecl( (yyloc), (yysemantic_stack_[(2) - (1)].cstr), (yysemantic_stack_[(2) - (2)].b) ) ) }
    break;

  case 87:

    { PARSE_EV( onTypeDecl( (yyloc), *(yysemantic_stack_[(2) - (1)].str), (yysemantic_stack_[(2) - (2)].b) ) ) }
    break;

  case 88:

    { (yyval.b) = false; }
    break;

  case 89:

    { (yyval.b) = true; }
    break;

  case 90:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), co::TK_INTERFACE, (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 93:

    { PARSE_EV( onBaseType( (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 98:

    { PARSE_EV( onCppBlock( (yylocation_stack_[(1) - (1)]), *(yysemantic_stack_[(1) - (1)].str) ) ) }
    break;

  case 100:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), co::TK_COMPONENT, (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 108:

    { PARSE_EV( onTypeDecl( (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str), false ) ) }
    break;

  case 109:

    { PARSE_EV( onPort( (yylocation_stack_[(5) - (4)]), (yysemantic_stack_[(5) - (1)].b), *(yysemantic_stack_[(5) - (4)].str) ) ) }
    break;

  case 110:

    { (yyval.b) = true; }
    break;

  case 111:

    { (yyval.b) = false; }
    break;

  case 112:

    { (yyval.any) = loader.newAny(); (yyval.any)->set<bool>( (yysemantic_stack_[(1) - (1)].b) ); }
    break;

  case 113:

    { (yyval.any) = loader.newAny(); (yyval.any)->set<const std::string&>( *(yysemantic_stack_[(1) - (1)].str) ); }
    break;

  case 114:

    { (yyval.any) = loader.newAny(); (yyval.any)->set<double>( (yysemantic_stack_[(1) - (1)].num) ); }
    break;

  case 115:

    { (yyval.num) = (yysemantic_stack_[(1) - (1)].num); }
    break;

  case 116:

    { (yyval.num) = (yysemantic_stack_[(3) - (2)].num); }
    break;

  case 117:

    { (yyval.num) = (yysemantic_stack_[(3) - (1)].num) + (yysemantic_stack_[(3) - (3)].num); }
    break;

  case 118:

    { (yyval.num) = (yysemantic_stack_[(3) - (1)].num) - (yysemantic_stack_[(3) - (3)].num); }
    break;

  case 119:

    { (yyval.num) = (yysemantic_stack_[(3) - (1)].num) * (yysemantic_stack_[(3) - (3)].num); }
    break;

  case 120:

    { (yyval.num) = (yysemantic_stack_[(3) - (1)].num) / (yysemantic_stack_[(3) - (3)].num); }
    break;

  case 121:

    { (yyval.num) = -(yysemantic_stack_[(2) - (2)].num); }
    break;



	default:
          break;
      }
    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[1] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (!yy_pact_value_is_default_ (yyn))
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (int, int)
  {
    return YY_("syntax error");
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char Parser::yypact_ninf_ = -79;
  const short int
  Parser::yypact_[] =
  {
        41,   -79,   -79,   -79,    -3,    -1,    18,    40,    20,    23,
      35,    54,    56,   -79,    28,   -79,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,    29,   -79,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,     2,   -79,    42,
      64,    94,    32,    68,   -79,    75,   -79,   -79,    83,    83,
      81,    99,   -79,   -79,   109,    16,   115,    40,    86,   117,
      85,     5,   -79,   101,   -79,   -79,    89,    83,    83,    83,
      83,   -79,   -79,   -79,    88,    16,   -79,   -79,    40,   -79,
     115,   122,   -79,   -27,   -79,    67,    36,    95,    70,   -79,
      95,   102,    85,   -79,   -79,   111,   -79,   -79,    99,   -79,
     -16,   -16,   -79,   -79,   -79,   -79,   -79,   -79,   -79,   115,
     -79,   -79,   -79,   -79,   -79,   -79,   111,   103,    67,   -79,
     -79,   -79,    93,   104,   -79,   111,   -79,   -79,   -79,   -79,
     -79,   -79,   110,    99,   133,   115,   -79,   111,    87,   -79,
     -79,   -79,   -79,   113,   -79,   114,   -79,   116,   -79,   -79,
     118,   -79,   -79,   -79,     3,    73,   -79,   -79,   -79,   -79,
     119,   112,   -79,    70,   120,    73,   -79,   126,   100,   140,
     -79,   -79,    40,   124,   -79,   -79,   -79,   125,   -79,   -79,
      40,   -79
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     3,    27,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     4,     0,     9,     6,     7,    10,    11,
      12,    13,    14,    15,    29,   100,    39,    45,    25,    26,
      16,    90,    56,    46,     1,     5,     8,     0,    28,     0,
       0,    92,     0,     0,   112,     0,   113,   115,     0,     0,
       0,    33,    36,    31,   114,   102,    20,     0,     0,     0,
      48,     0,   121,     0,    30,    37,    34,     0,     0,     0,
       0,   110,   111,   106,     0,   103,   104,   107,     0,    18,
      21,     0,    43,     0,    93,    94,     0,    88,     0,    52,
      88,     0,    49,    50,    53,     0,    38,   116,     0,    32,
     118,   117,   119,   120,   101,   105,   108,    19,    41,    20,
      40,    98,    68,    66,    99,    65,     0,     0,    95,    96,
      60,    59,     0,     0,    86,     0,    87,    47,    51,    23,
      22,    24,     0,    35,     0,    20,    44,     0,     0,    91,
      97,    57,    89,     0,    54,     0,    42,     0,    70,    67,
       0,    55,   109,    69,     0,    61,    80,    77,    79,    78,
       0,    73,    74,     0,     0,    62,    63,    81,     0,     0,
      58,    64,     0,     0,    75,    76,    85,    82,    83,    71,
       0,    84
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
       -79,   -79,   -79,   142,   -79,   -79,     1,   -79,    22,   -37,
      -7,   -79,   -79,   -79,   -79,   -79,   -79,    57,   -48,   -79,
     -79,    51,   -79,   -79,   -79,   -79,   -79,   -79,   -79,   -55,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,   -78,   -79,   -79,
      15,   -79,   -79,   -79,    -4,   -79,   -79,   -79,   -14,   -58,
      77,   -79,   -79,   -79,   -79,   -79,    50,   -79,   -79,   -79,
     -79,    96,   -79,   -79,   -79,   108,   -41
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,    11,    12,    13,    14,    15,    89,    80,    81,   132,
      90,    17,    24,    38,    50,    66,    99,    51,    52,    18,
      40,    82,   135,    83,    19,    20,    43,    91,    92,   113,
      94,    21,    42,   150,   122,   164,   165,   114,   115,   137,
     149,   154,   160,   161,   162,   163,   173,   177,   178,   116,
     124,    22,    41,    58,   117,   118,   119,    23,    39,    74,
      75,    76,    77,   134,    78,    53,    54
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char Parser::yytable_ninf_ = -73;
  const short int
  Parser::yytable_[] =
  {
        30,    16,    95,    65,   156,    93,    44,    62,    63,    44,
      25,   109,    26,    16,   110,    45,    69,    70,   157,   158,
      46,     3,    47,    46,   159,    47,   100,   101,   102,   103,
     125,    27,    48,    31,    95,    48,    32,   128,    49,    71,
     -72,    49,     1,    72,     2,   120,     3,     4,    33,   121,
      84,     5,     6,    28,    34,     7,    73,    79,     8,     2,
       9,     3,     4,    36,    29,    37,     5,     6,    59,    10,
       7,   106,     3,     8,   111,     9,    73,   166,     3,   138,
      28,   107,    55,    28,    10,    65,    28,   171,   143,    87,
       3,    29,    87,    88,    29,    87,   112,    29,    28,    88,
     147,   156,   112,    47,    56,   169,    57,    87,    60,    29,
      79,    88,    45,    48,    61,   157,   158,   129,    64,    49,
       3,   159,   144,   148,   130,    86,    85,    98,   131,   104,
     141,    67,    68,    69,    70,   108,    79,   123,    97,    67,
      68,    69,    70,   127,   139,   144,   145,   142,   151,   152,
     168,   172,   148,   175,    35,   133,   167,   146,   155,   179,
     136,   170,   153,   180,   174,   176,   181,   126,   140,    96,
       0,   105,     0,   176
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
         7,     0,    60,    51,     1,    60,     4,    48,    49,     4,
      13,    38,    13,    12,    41,    13,    32,    33,    15,    16,
      18,     5,    20,    18,    21,    20,    67,    68,    69,    70,
      88,    13,    30,    13,    92,    30,    13,    92,    36,    23,
      37,    36,     1,    27,     3,     9,     5,     6,    13,    13,
      57,    10,    11,    13,     0,    14,    55,    56,    17,     3,
      19,     5,     6,    35,    24,    36,    10,    11,    36,    28,
      14,    78,     5,    17,     7,    19,    75,   155,     5,   116,
      13,    80,    40,    13,    28,   133,    13,   165,   125,    22,
       5,    24,    22,    26,    24,    22,    29,    24,    13,    26,
     137,     1,    29,    20,    40,   163,    12,    22,    40,    24,
     109,    26,    13,    30,    39,    15,    16,     6,    37,    36,
       5,    21,    35,    36,    13,     8,    40,    38,    17,    41,
      37,    30,    31,    32,    33,    13,   135,    42,    37,    30,
      31,    32,    33,    41,    41,    35,    13,    43,    35,    35,
      38,    25,    36,    13,    12,    98,    37,   135,    40,    35,
     109,    41,   147,    38,   168,   172,   180,    90,   118,    61,
      -1,    75,    -1,   180
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     1,     3,     5,     6,    10,    11,    14,    17,    19,
      28,    45,    46,    47,    48,    49,    50,    55,    63,    68,
      69,    75,    95,   101,    56,    13,    13,    13,    13,    24,
      54,    13,    13,    13,     0,    47,    35,    36,    57,   102,
      64,    96,    76,    70,     4,    13,    18,    20,    30,    36,
      58,    61,    62,   109,   110,    40,    40,    12,    97,    36,
      40,    39,   110,   110,    37,    62,    59,    30,    31,    32,
      33,    23,    27,    50,   103,   104,   105,   106,   108,    50,
      51,    52,    65,    67,    54,    40,     8,    22,    26,    50,
      54,    71,    72,    73,    74,    93,   109,    37,    38,    60,
     110,   110,   110,   110,    41,   105,    54,    50,    13,    38,
      41,     7,    29,    73,    81,    82,    93,    98,    99,   100,
       9,    13,    78,    42,    94,    93,    94,    41,    73,     6,
      13,    17,    53,    61,   107,    66,    65,    83,    53,    41,
     100,    37,    43,    53,    35,    13,    52,    53,    36,    84,
      77,    35,    35,    84,    85,    40,     1,    15,    16,    21,
      86,    87,    88,    89,    79,    80,    81,    37,    38,    93,
      41,    81,    25,    90,    88,    13,    54,    91,    92,    35,
      38,    92
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
      45,    43,    42,    47,   285,    59,    40,    41,    44,    61,
     123,   125,    91,    93
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    44,    45,    45,    46,    46,    47,    47,    47,    48,
      48,    48,    48,    48,    48,    48,    49,    50,    51,    51,
      52,    52,    53,    53,    53,    54,    54,    56,    55,    57,
      57,    59,    58,    58,    60,    60,    61,    61,    62,    64,
      63,    66,    65,    67,    67,    68,    70,    69,    71,    71,
      72,    72,    73,    73,    74,    74,    76,    77,    75,    78,
      78,    79,    79,    80,    80,    81,    81,    82,    83,    82,
      85,    84,    86,    86,    87,    87,    88,    89,    89,    89,
      89,    90,    90,    91,    91,    92,    93,    93,    94,    94,
      96,    95,    97,    97,    98,    98,    99,    99,   100,   100,
     102,   101,   103,   103,   104,   104,   105,   105,   107,   106,
     108,   108,   109,   109,   109,   110,   110,   110,   110,   110,
     110,   110
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     1,     1,     2,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     2,
       0,     1,     1,     1,     1,     1,     1,     0,     3,     0,
       3,     0,     3,     1,     0,     2,     1,     2,     3,     0,
       6,     0,     4,     1,     3,     2,     0,     6,     0,     1,
       1,     2,     1,     1,     3,     4,     0,     0,    11,     1,
       1,     0,     1,     1,     2,     1,     1,     3,     0,     4,
       0,     6,     0,     1,     1,     3,     3,     1,     1,     1,
       1,     0,     2,     1,     3,     1,     2,     2,     0,     2,
       0,     7,     0,     2,     0,     1,     1,     2,     1,     1,
       0,     6,     0,     1,     1,     2,     1,     1,     0,     5,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "\"annotation identifier\"",
  "\"boolean value\"", "\"comment\"", "\"component\"", "\"C++ block\"",
  "\"C++ tag\"", "\"C++ type\"", "\"enum\"", "\"exception\"",
  "\"extends\"", "\"identifier\"", "\"import\"", "\"in\"", "\"inout\"",
  "\"interface\"", "\"string literal\"", "\"native class\"", "\"number\"",
  "\"out\"", "\"primitive type\"", "\"provides\"",
  "\"qualified identifier\"", "\"raises\"", "\"readonly\"", "\"receives\"",
  "\"struct\"", "\"void\"", "'-'", "'+'", "'*'", "'/'", "NEG", "';'",
  "'('", "')'", "','", "'='", "'{'", "'}'", "'['", "']'", "$accept",
  "csl_file", "sentence_list", "sentence", "statement", "import_clause",
  "comment", "comment_list", "opt_comment_list", "identifier",
  "qualified_id", "annotation", "$@1", "opt_annotation_data",
  "annotation_data", "$@2", "opt_field_value_list", "field_value_list",
  "field_value", "enum_spec", "$@3", "enum_identifier", "$@4",
  "enum_identifier_list", "exception_spec", "struct_spec", "$@5",
  "opt_record_member_list", "record_member_list", "record_member",
  "field_decl", "nativeclass_spec", "$@6", "$@7", "cpp_type",
  "opt_class_member_list", "class_member_list", "class_member",
  "method_decl", "$@8", "method_remaining_parts", "$@9", "opt_param_list",
  "param_list", "param", "inout", "opt_exception_list", "exception_list",
  "exception", "type_decl", "opt_array", "interface_spec", "$@10",
  "opt_inheritance_decl", "opt_interface_member_list",
  "interface_member_list", "interface_member", "component_spec", "$@11",
  "opt_component_member_list", "component_member_list", "component_member",
  "port_decl", "$@12", "port_kind", "exp", "num_exp", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        45,     0,    -1,    46,    -1,     1,    -1,    47,    -1,    46,
      47,    -1,    50,    -1,    55,    -1,    48,    35,    -1,    49,
      -1,    63,    -1,    68,    -1,    69,    -1,    75,    -1,    95,
      -1,   101,    -1,    14,    54,    -1,     5,    -1,    50,    -1,
      51,    50,    -1,    -1,    51,    -1,    13,    -1,     6,    -1,
      17,    -1,    13,    -1,    24,    -1,    -1,     3,    56,    57,
      -1,    -1,    36,    58,    37,    -1,    -1,   109,    59,    60,
      -1,    61,    -1,    -1,    38,    61,    -1,    62,    -1,    61,
      62,    -1,    13,    39,   109,    -1,    -1,    10,    13,    64,
      40,    67,    41,    -1,    -1,    52,    13,    66,    52,    -1,
      65,    -1,    67,    38,    65,    -1,    11,    13,    -1,    -1,
      28,    13,    70,    40,    71,    41,    -1,    -1,    72,    -1,
      73,    -1,    72,    73,    -1,    50,    -1,    74,    -1,    93,
      53,    35,    -1,    26,    93,    53,    35,    -1,    -1,    -1,
      19,    13,    76,    36,     8,    78,    37,    77,    40,    79,
      41,    -1,    13,    -1,     9,    -1,    -1,    80,    -1,    81,
      -1,    80,    81,    -1,    82,    -1,    73,    -1,    93,    53,
      84,    -1,    -1,    29,    83,    53,    84,    -1,    -1,    36,
      85,    86,    37,    90,    35,    -1,    -1,    87,    -1,    88,
      -1,    87,    38,    88,    -1,    89,    93,    13,    -1,    15,
      -1,    21,    -1,    16,    -1,     1,    -1,    -1,    25,    91,
      -1,    92,    -1,    91,    38,    92,    -1,    54,    -1,    22,
      94,    -1,    54,    94,    -1,    -1,    42,    43,    -1,    -1,
      17,    13,    96,    97,    40,    98,    41,    -1,    -1,    12,
      54,    -1,    -1,    99,    -1,   100,    -1,    99,   100,    -1,
       7,    -1,    81,    -1,    -1,     6,    13,   102,    40,   103,
      41,    -1,    -1,   104,    -1,   105,    -1,   104,   105,    -1,
      50,    -1,   106,    -1,    -1,   108,    54,   107,    13,    35,
      -1,    23,    -1,    27,    -1,     4,    -1,    18,    -1,   110,
      -1,    20,    -1,    36,   110,    37,    -1,   110,    31,   110,
      -1,   110,    30,   110,    -1,   110,    32,   110,    -1,   110,
      33,   110,    -1,    30,   110,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     7,     9,    12,    14,    16,    19,
      21,    23,    25,    27,    29,    31,    33,    36,    38,    40,
      43,    44,    46,    48,    50,    52,    54,    56,    57,    61,
      62,    66,    67,    71,    73,    74,    77,    79,    82,    86,
      87,    94,    95,   100,   102,   106,   109,   110,   117,   118,
     120,   122,   125,   127,   129,   133,   138,   139,   140,   152,
     154,   156,   157,   159,   161,   164,   166,   168,   172,   173,
     178,   179,   186,   187,   189,   191,   195,   199,   201,   203,
     205,   207,   208,   211,   213,   217,   219,   222,   225,   226,
     229,   230,   238,   239,   242,   243,   245,   247,   250,   252,
     254,   255,   262,   263,   265,   267,   270,   272,   274,   275,
     281,   283,   285,   287,   289,   291,   293,   297,   301,   305,
     309,   313
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   112,   112,   113,   127,   128,   132,   133,   134,   138,
     139,   140,   141,   142,   143,   144,   148,   152,   156,   157,
     160,   162,   166,   167,   168,   172,   173,   177,   177,   180,
     182,   186,   186,   187,   190,   192,   196,   197,   201,   205,
     205,   210,   210,   214,   215,   219,   223,   223,   227,   229,
     233,   234,   238,   239,   243,   244,   248,   249,   248,   254,
     255,   258,   260,   264,   265,   269,   270,   274,   275,   275,
     279,   279,   283,   285,   289,   290,   294,   299,   300,   301,
     302,   306,   308,   312,   313,   317,   321,   322,   326,   327,
     331,   331,   335,   337,   340,   342,   346,   347,   351,   352,
     356,   356,   360,   362,   366,   367,   371,   372,   376,   376,
     381,   382,   386,   387,   388,   392,   393,   394,   395,   396,
     397,   398
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      36,    37,    32,    31,    38,    30,     2,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    35,
       2,    39,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    42,     2,    43,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,     2,    41,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    34
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 173;
  const int Parser::yynnts_ = 67;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 34;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 44;

  const unsigned int Parser::yyuser_token_number_max_ = 285;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


} } // co::csl





void co::csl::Parser::error( const location_type&, const std::string& )
{
	// ignore
}

