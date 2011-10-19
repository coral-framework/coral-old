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

    { PARSE_EV( onAnnotation( (yylocation_stack_[(1) - (1)]), *(yysemantic_stack_[(1) - (1)].str), false ) ) }
    break;

  case 28:

    { PARSE_EV( onAnnotation( (yylocation_stack_[(2) - (1)]), *(yysemantic_stack_[(2) - (1)].str), true ) ) }
    break;

  case 30:

    { PARSE_EV( onAnnotationData( (yylocation_stack_[(1) - (1)]), "value", *(yysemantic_stack_[(1) - (1)].any) ) ) }
    break;

  case 37:

    { PARSE_EV( onAnnotationData( (yyloc), *(yysemantic_stack_[(3) - (1)].str), *(yysemantic_stack_[(3) - (3)].any) ) ) }
    break;

  case 38:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), co::TK_ENUM, (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 40:

    { PARSE_EV( onEnumIdentifier( (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 44:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), co::TK_EXCEPTION, (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 45:

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

  case 109:

    { PARSE_EV( onTypeDecl( (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str), false ) ) }
    break;

  case 110:

    { PARSE_EV( onPort( (yylocation_stack_[(5) - (4)]), (yysemantic_stack_[(5) - (1)].b), *(yysemantic_stack_[(5) - (4)].str) ) ) }
    break;

  case 111:

    { (yyval.b) = true; }
    break;

  case 112:

    { (yyval.b) = false; }
    break;

  case 113:

    { (yyval.any) = loader.newAny(); (yyval.any)->set<bool>( (yysemantic_stack_[(1) - (1)].b) ); }
    break;

  case 114:

    { (yyval.any) = loader.newAny(); (yyval.any)->set<const std::string&>( *(yysemantic_stack_[(1) - (1)].str) ); }
    break;

  case 115:

    { (yyval.any) = loader.newAny(); (yyval.any)->set<const std::string&>( *(yysemantic_stack_[(1) - (1)].str) ); }
    break;

  case 116:

    { (yyval.any) = loader.newAny(); (yyval.any)->set<double>( (yysemantic_stack_[(1) - (1)].num) ); }
    break;

  case 117:

    { (yyval.num) = (yysemantic_stack_[(1) - (1)].num); }
    break;

  case 118:

    { (yyval.num) = (yysemantic_stack_[(3) - (2)].num); }
    break;

  case 119:

    { (yyval.num) = (yysemantic_stack_[(3) - (1)].num) + (yysemantic_stack_[(3) - (3)].num); }
    break;

  case 120:

    { (yyval.num) = (yysemantic_stack_[(3) - (1)].num) - (yysemantic_stack_[(3) - (3)].num); }
    break;

  case 121:

    { (yyval.num) = (yysemantic_stack_[(3) - (1)].num) * (yysemantic_stack_[(3) - (3)].num); }
    break;

  case 122:

    { (yyval.num) = (yysemantic_stack_[(3) - (1)].num) / (yysemantic_stack_[(3) - (3)].num); }
    break;

  case 123:

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
  const short int Parser::yypact_ninf_ = -148;
  const short int
  Parser::yypact_[] =
  {
        84,  -148,   -30,  -148,    32,    35,    46,    -3,    54,    58,
      60,    75,   105,  -148,    43,  -148,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,    19,    44,    48,
      81,    61,    59,  -148,    66,  -148,  -148,   -12,   -12,  -148,
      69,    83,  -148,  -148,   130,     6,    91,    -3,    73,   109,
     115,    56,  -148,   121,  -148,   110,    87,   -12,   -12,   -12,
     -12,  -148,  -148,  -148,  -148,    88,     6,  -148,  -148,    -3,
    -148,    91,   118,  -148,   -24,  -148,    39,    57,    90,    40,
    -148,    90,  -148,    93,   115,  -148,  -148,    34,  -148,  -148,
      66,  -148,   110,  -148,    -6,    -6,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,    91,  -148,  -148,  -148,  -148,  -148,  -148,
      34,    94,    39,  -148,  -148,  -148,    99,    95,  -148,    34,
    -148,  -148,  -148,  -148,  -148,  -148,   107,    83,   142,    91,
    -148,    34,    -1,  -148,  -148,  -148,  -148,   108,  -148,   122,
    -148,   120,  -148,  -148,   119,  -148,  -148,  -148,     4,    78,
    -148,  -148,  -148,  -148,   127,   128,  -148,    40,   124,    78,
    -148,   143,   129,   154,  -148,  -148,    -3,   135,  -148,  -148,
    -148,   133,  -148,  -148,    -3,  -148
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     3,    27,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     4,     0,     9,     6,     7,    10,    11,
      12,    13,    14,    15,    28,   100,    38,    44,    25,    26,
      16,    90,    56,    45,     1,     5,     8,     0,     0,     0,
      92,     0,     0,   113,    25,   114,   117,     0,     0,   115,
       0,    32,    35,    30,   116,   102,    20,     0,     0,     0,
      47,     0,   123,     0,    29,     0,    33,     0,     0,     0,
       0,   111,   112,   106,   107,     0,   103,   104,   108,     0,
      18,    21,     0,    42,     0,    93,    94,     0,    88,     0,
      51,    88,    52,     0,    48,    49,    53,     0,    37,   118,
       0,    36,     0,    31,   120,   119,   121,   122,   101,   105,
     109,    19,    40,    20,    39,    98,    68,    66,    99,    65,
       0,     0,    95,    96,    60,    59,     0,     0,    86,     0,
      87,    46,    50,    23,    22,    24,     0,    34,     0,    20,
      43,     0,     0,    91,    97,    57,    89,     0,    54,     0,
      41,     0,    70,    67,     0,    55,   110,    69,     0,    61,
      80,    77,    79,    78,     0,    73,    74,     0,     0,    62,
      63,    81,     0,     0,    58,    64,     0,     0,    75,    76,
      85,    82,    83,    71,     0,    84
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -148,  -148,  -148,   160,  -148,  -148,     1,  -148,    36,  -113,
      -7,     3,  -148,  -148,  -148,  -148,    71,   111,  -148,  -148,
      65,  -148,  -148,  -148,  -148,  -148,  -148,  -148,   -56,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -147,  -148,  -148,    23,
    -148,  -148,  -148,     7,  -148,  -148,  -148,    -4,   -58,    92,
    -148,  -148,  -148,  -148,  -148,    62,  -148,  -148,  -148,  -148,
     106,  -148,  -148,  -148,   125,    79
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,    11,    12,    13,    14,    15,    90,    81,    82,   136,
      91,    92,    37,    50,    66,   103,    51,    52,    18,    39,
      83,   139,    84,    19,    20,    42,    93,    94,   117,    96,
      21,    41,   154,   126,   168,   169,   118,   119,   141,   153,
     158,   164,   165,   166,   167,   177,   181,   182,   120,   128,
      22,    40,    58,   121,   122,   123,    23,    38,    75,    76,
      77,    78,   138,    79,    53,    54
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char Parser::yytable_ninf_ = -73;
  const short int
  Parser::yytable_[] =
  {
        30,    16,    97,    17,    95,   160,    24,   142,    46,     2,
      28,     3,   170,    16,   113,    17,   147,   114,    47,   161,
     162,    29,   175,    43,    48,   163,    69,    70,   151,    71,
      49,   129,    44,    72,   148,   152,    97,    45,   132,    46,
     133,   -72,     2,    29,     3,    25,   115,   134,    26,    47,
      85,   135,    28,    28,    49,    48,    73,    80,    74,    27,
      43,    88,    88,    29,    29,    89,   124,    31,   116,    28,
     125,    32,   110,    33,    45,    34,    46,    73,    36,    74,
      29,     2,   111,     3,    55,     1,    47,     2,    56,     3,
       4,    28,    48,    57,     5,     6,     3,    59,     7,    60,
      88,     8,    29,     9,    89,    61,    64,   116,     2,   173,
       3,     4,    10,    86,    80,     5,     6,    87,     2,     7,
       3,    65,     8,   100,     9,   102,    62,    63,    28,   108,
     160,   112,   127,    10,   131,   143,   145,    88,   146,    29,
      80,    89,   148,   155,   161,   162,   104,   105,   106,   107,
     163,    67,    68,    69,    70,   149,   152,   156,    99,   159,
      67,    68,    69,    70,   171,   174,   172,   179,   176,   180,
     183,   184,    35,   137,   157,   150,   101,   180,   140,   178,
     185,     0,   109,   130,   144,     0,    98
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
         7,     0,    60,     0,    60,     1,    36,   120,    20,     3,
      13,     5,   159,    12,    38,    12,   129,    41,    30,    15,
      16,    24,   169,     4,    36,    21,    32,    33,   141,    23,
      37,    89,    13,    27,    35,    36,    94,    18,    94,    20,
       6,    37,     3,    24,     5,    13,     7,    13,    13,    30,
      57,    17,    13,    13,    61,    36,    55,    56,    55,    13,
       4,    22,    22,    24,    24,    26,     9,    13,    29,    13,
      13,    13,    79,    13,    18,     0,    20,    76,    35,    76,
      24,     3,    81,     5,    40,     1,    30,     3,    40,     5,
       6,    13,    36,    12,    10,    11,     5,    36,    14,    40,
      22,    17,    24,    19,    26,    39,    37,    29,     3,   167,
       5,     6,    28,    40,   113,    10,    11,     8,     3,    14,
       5,    38,    17,    13,    19,    38,    47,    48,    13,    41,
       1,    13,    42,    28,    41,    41,    37,    22,    43,    24,
     139,    26,    35,    35,    15,    16,    67,    68,    69,    70,
      21,    30,    31,    32,    33,    13,    36,    35,    37,    40,
      30,    31,    32,    33,    37,    41,    38,    13,    25,   176,
      35,    38,    12,   102,   151,   139,    65,   184,   113,   172,
     184,    -1,    76,    91,   122,    -1,    61
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     1,     3,     5,     6,    10,    11,    14,    17,    19,
      28,    45,    46,    47,    48,    49,    50,    55,    62,    67,
      68,    74,    94,   100,    36,    13,    13,    13,    13,    24,
      54,    13,    13,    13,     0,    47,    35,    56,   101,    63,
      95,    75,    69,     4,    13,    18,    20,    30,    36,    54,
      57,    60,    61,   108,   109,    40,    40,    12,    96,    36,
      40,    39,   109,   109,    37,    38,    58,    30,    31,    32,
      33,    23,    27,    50,    55,   102,   103,   104,   105,   107,
      50,    51,    52,    64,    66,    54,    40,     8,    22,    26,
      50,    54,    55,    70,    71,    72,    73,    92,   108,    37,
      13,    61,    38,    59,   109,   109,   109,   109,    41,   104,
      54,    50,    13,    38,    41,     7,    29,    72,    80,    81,
      92,    97,    98,    99,     9,    13,    77,    42,    93,    92,
      93,    41,    72,     6,    13,    17,    53,    60,   106,    65,
      64,    82,    53,    41,    99,    37,    43,    53,    35,    13,
      52,    53,    36,    83,    76,    35,    35,    83,    84,    40,
       1,    15,    16,    21,    85,    86,    87,    88,    78,    79,
      80,    37,    38,    92,    41,    80,    25,    89,    87,    13,
      54,    90,    91,    35,    38,    91
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
      52,    52,    53,    53,    53,    54,    54,    55,    56,    55,
      58,    57,    57,    59,    59,    60,    60,    61,    63,    62,
      65,    64,    66,    66,    67,    69,    68,    70,    70,    71,
      71,    72,    72,    72,    73,    73,    75,    76,    74,    77,
      77,    78,    78,    79,    79,    80,    80,    81,    82,    81,
      84,    83,    85,    85,    86,    86,    87,    88,    88,    88,
      88,    89,    89,    90,    90,    91,    92,    92,    93,    93,
      95,    94,    96,    96,    97,    97,    98,    98,    99,    99,
     101,   100,   102,   102,   103,   103,   104,   104,   104,   106,
     105,   107,   107,   108,   108,   108,   108,   109,   109,   109,
     109,   109,   109,   109
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     1,     1,     2,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     2,
       0,     1,     1,     1,     1,     1,     1,     1,     0,     5,
       0,     3,     1,     0,     2,     1,     3,     3,     0,     6,
       0,     4,     1,     3,     2,     0,     6,     0,     1,     1,
       2,     1,     1,     1,     3,     4,     0,     0,    11,     1,
       1,     0,     1,     1,     2,     1,     1,     3,     0,     4,
       0,     6,     0,     1,     1,     3,     3,     1,     1,     1,
       1,     0,     2,     1,     3,     1,     2,     2,     0,     2,
       0,     7,     0,     2,     0,     1,     1,     2,     1,     1,
       0,     6,     0,     1,     1,     2,     1,     1,     1,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     2
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
  "qualified_id", "annotation", "$@1", "annotation_data", "$@2",
  "opt_field_value_list", "field_value_list", "field_value", "enum_spec",
  "$@3", "enum_identifier", "$@4", "enum_identifier_list",
  "exception_spec", "struct_spec", "$@5", "opt_record_member_list",
  "record_member_list", "record_member", "field_decl", "nativeclass_spec",
  "$@6", "$@7", "cpp_type", "opt_class_member_list", "class_member_list",
  "class_member", "method_decl", "$@8", "method_remaining_parts", "$@9",
  "opt_param_list", "param_list", "param", "inout", "opt_exception_list",
  "exception_list", "exception", "type_decl", "opt_array",
  "interface_spec", "$@10", "opt_inheritance_decl",
  "opt_interface_member_list", "interface_member_list", "interface_member",
  "component_spec", "$@11", "opt_component_member_list",
  "component_member_list", "component_member", "port_decl", "$@12",
  "port_kind", "exp", "num_exp", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        45,     0,    -1,    46,    -1,     1,    -1,    47,    -1,    46,
      47,    -1,    50,    -1,    55,    -1,    48,    35,    -1,    49,
      -1,    62,    -1,    67,    -1,    68,    -1,    74,    -1,    94,
      -1,   100,    -1,    14,    54,    -1,     5,    -1,    50,    -1,
      51,    50,    -1,    -1,    51,    -1,    13,    -1,     6,    -1,
      17,    -1,    13,    -1,    24,    -1,     3,    -1,    -1,     3,
      36,    56,    57,    37,    -1,    -1,   108,    58,    59,    -1,
      60,    -1,    -1,    38,    60,    -1,    61,    -1,    60,    38,
      61,    -1,    13,    39,   108,    -1,    -1,    10,    13,    63,
      40,    66,    41,    -1,    -1,    52,    13,    65,    52,    -1,
      64,    -1,    66,    38,    64,    -1,    11,    13,    -1,    -1,
      28,    13,    69,    40,    70,    41,    -1,    -1,    71,    -1,
      72,    -1,    71,    72,    -1,    50,    -1,    55,    -1,    73,
      -1,    92,    53,    35,    -1,    26,    92,    53,    35,    -1,
      -1,    -1,    19,    13,    75,    36,     8,    77,    37,    76,
      40,    78,    41,    -1,    13,    -1,     9,    -1,    -1,    79,
      -1,    80,    -1,    79,    80,    -1,    81,    -1,    72,    -1,
      92,    53,    83,    -1,    -1,    29,    82,    53,    83,    -1,
      -1,    36,    84,    85,    37,    89,    35,    -1,    -1,    86,
      -1,    87,    -1,    86,    38,    87,    -1,    88,    92,    13,
      -1,    15,    -1,    21,    -1,    16,    -1,     1,    -1,    -1,
      25,    90,    -1,    91,    -1,    90,    38,    91,    -1,    54,
      -1,    22,    93,    -1,    54,    93,    -1,    -1,    42,    43,
      -1,    -1,    17,    13,    95,    96,    40,    97,    41,    -1,
      -1,    12,    54,    -1,    -1,    98,    -1,    99,    -1,    98,
      99,    -1,     7,    -1,    80,    -1,    -1,     6,    13,   101,
      40,   102,    41,    -1,    -1,   103,    -1,   104,    -1,   103,
     104,    -1,    50,    -1,    55,    -1,   105,    -1,    -1,   107,
      54,   106,    13,    35,    -1,    23,    -1,    27,    -1,     4,
      -1,    18,    -1,    54,    -1,   109,    -1,    20,    -1,    36,
     109,    37,    -1,   109,    31,   109,    -1,   109,    30,   109,
      -1,   109,    32,   109,    -1,   109,    33,   109,    -1,    30,
     109,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     7,     9,    12,    14,    16,    19,
      21,    23,    25,    27,    29,    31,    33,    36,    38,    40,
      43,    44,    46,    48,    50,    52,    54,    56,    58,    59,
      65,    66,    70,    72,    73,    76,    78,    82,    86,    87,
      94,    95,   100,   102,   106,   109,   110,   117,   118,   120,
     122,   125,   127,   129,   131,   135,   140,   141,   142,   154,
     156,   158,   159,   161,   163,   166,   168,   170,   174,   175,
     180,   181,   188,   189,   191,   193,   197,   201,   203,   205,
     207,   209,   210,   213,   215,   219,   221,   224,   227,   228,
     231,   232,   240,   241,   244,   245,   247,   249,   252,   254,
     256,   257,   264,   265,   267,   269,   272,   274,   276,   278,
     279,   285,   287,   289,   291,   293,   295,   297,   299,   303,
     307,   311,   315,   319
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   112,   112,   113,   127,   128,   132,   133,   134,   138,
     139,   140,   141,   142,   143,   144,   148,   152,   156,   157,
     160,   162,   166,   167,   168,   172,   173,   177,   178,   178,
     182,   182,   183,   186,   188,   192,   193,   197,   201,   201,
     206,   206,   210,   211,   215,   219,   219,   223,   225,   229,
     230,   234,   235,   236,   240,   241,   245,   246,   245,   251,
     252,   255,   257,   261,   262,   266,   267,   271,   272,   272,
     276,   276,   280,   282,   286,   287,   291,   296,   297,   298,
     299,   303,   305,   309,   310,   314,   318,   319,   323,   324,
     328,   328,   332,   334,   337,   339,   343,   344,   348,   349,
     353,   353,   357,   359,   363,   364,   368,   369,   370,   374,
     374,   379,   380,   384,   385,   386,   387,   391,   392,   393,
     394,   395,   396,   397
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
  const int Parser::yylast_ = 186;
  const int Parser::yynnts_ = 66;
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

