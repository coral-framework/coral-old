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
				// lex error
			}
			else
			{
				PUSH_ERROR( yylloc, "syntax error near '" << csl_get_text( scanner ) << "'" )
			}
			YYABORT;
		}
    break;

  case 14:

    { PARSE_EV( onComment( (yylocation_stack_[(1) - (1)]), *(yysemantic_stack_[(1) - (1)].str) ) ) }
    break;

  case 19:

    { PARSE_EV( onImport( (yylocation_stack_[(3) - (1)]), *(yysemantic_stack_[(3) - (2)].str) ) ) }
    break;

  case 20:

    { (yyval.str) = (yysemantic_stack_[(1) - (1)].str); }
    break;

  case 21:

    { static std::string sComponent( "component" ); (yyval.str) = &sComponent; }
    break;

  case 22:

    { static std::string sInterface( "interface" ); (yyval.str) = &sInterface; }
    break;

  case 25:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str), co::TK_ENUM ) ) }
    break;

  case 27:

    { PARSE_EV( onEnumIdentifier( (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 31:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(3) - (1)]), (yylocation_stack_[(3) - (2)]), *(yysemantic_stack_[(3) - (2)].str), co::TK_EXCEPTION ) ) }
    break;

  case 32:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str), co::TK_STRUCT ) ) }
    break;

  case 40:

    { PARSE_EV( onField( (yylocation_stack_[(3) - (2)]), *(yysemantic_stack_[(3) - (2)].str), false ) ) }
    break;

  case 41:

    { PARSE_EV( onField( (yylocation_stack_[(4) - (3)]), *(yysemantic_stack_[(4) - (3)].str), true ) ) }
    break;

  case 42:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str), co::TK_NATIVECLASS ) ) }
    break;

  case 43:

    { PARSE_EV( onNativeClass( (yylocation_stack_[(7) - (4)]), *(yysemantic_stack_[(7) - (5)].str), *(yysemantic_stack_[(7) - (6)].str) ) ) }
    break;

  case 45:

    { (yyval.str) = (yysemantic_stack_[(1) - (1)].str); }
    break;

  case 46:

    { (yyval.str) = (yysemantic_stack_[(1) - (1)].str); }
    break;

  case 54:

    { PARSE_EV( onTypeDecl( (yylocation_stack_[(1) - (1)]), "void", false ) ) }
    break;

  case 56:

    { PARSE_EV( onMethod( (yylocation_stack_[(1) - (0)]), *(yysemantic_stack_[(1) - (0)].str) ) ) }
    break;

  case 57:

    { PARSE_EV( onEndMethod( (yylocation_stack_[(6) - (0)]) ) ) }
    break;

  case 62:

    { int v = (yysemantic_stack_[(3) - (1)].i32);
		PARSE_EV( onParameter( (yylocation_stack_[(3) - (3)]), ( v & 1 ) != 0, ( v & 2 ) != 0, *(yysemantic_stack_[(3) - (3)].str) ) ) }
    break;

  case 63:

    { (yyval.i32) = 1; }
    break;

  case 64:

    { (yyval.i32) = 2; }
    break;

  case 65:

    { (yyval.i32) = 3; }
    break;

  case 66:

    { PUSH_ERROR( (yyloc), "expected 'in', 'out' or 'inout' before '"
		<< csl_get_text( scanner ) << "'" ); YYABORT; }
    break;

  case 71:

    { PARSE_EV( onRaises( (yylocation_stack_[(1) - (1)]), *(yysemantic_stack_[(1) - (1)].str) ) ) }
    break;

  case 72:

    { PARSE_EV( onTypeDecl( (yyloc), (yysemantic_stack_[(2) - (1)].cstr), (yysemantic_stack_[(2) - (2)].b) ) ) }
    break;

  case 73:

    { PARSE_EV( onTypeDecl( (yyloc), *(yysemantic_stack_[(2) - (1)].str), (yysemantic_stack_[(2) - (2)].b) ) ) }
    break;

  case 74:

    { (yyval.b) = false; }
    break;

  case 75:

    { (yyval.b) = true; }
    break;

  case 76:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str), co::TK_INTERFACE ) ) }
    break;

  case 79:

    { PARSE_EV( onBaseType( (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str) ) ) }
    break;

  case 84:

    { PARSE_EV( onCppBlock( (yylocation_stack_[(1) - (1)]), *(yysemantic_stack_[(1) - (1)].str) ) ) }
    break;

  case 86:

    { PARSE_EV( onTypeSpec( (yylocation_stack_[(2) - (1)]), (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str), co::TK_COMPONENT ) ) }
    break;

  case 93:

    { PARSE_EV( onTypeDecl( (yylocation_stack_[(2) - (2)]), *(yysemantic_stack_[(2) - (2)].str), false ) ) }
    break;

  case 94:

    { PARSE_EV( onPort( (yylocation_stack_[(5) - (4)]), (yysemantic_stack_[(5) - (1)].b), *(yysemantic_stack_[(5) - (4)].str) ) ) }
    break;

  case 95:

    { (yyval.b) = true; }
    break;

  case 96:

    { (yyval.b) = false; }
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
  const signed char Parser::yypact_ninf_ = -116;
  const signed char
  Parser::yypact_[] =
  {
        19,  -116,  -116,    -2,    18,    21,   -10,    29,    31,    40,
      55,    69,  -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,
    -116,  -116,  -116,    27,  -116,  -116,    36,  -116,  -116,  -116,
    -116,  -116,    22,    28,  -116,  -116,    66,    42,    48,     5,
      77,   -10,    51,    83,    72,  -116,  -116,  -116,    56,     5,
    -116,   -10,  -116,    77,    79,  -116,    23,  -116,    44,    -4,
      54,    -7,  -116,    54,    62,    72,  -116,  -116,    46,    65,
    -116,  -116,  -116,  -116,    67,    77,  -116,  -116,  -116,  -116,
    -116,    46,    70,    44,  -116,  -116,  -116,    63,    64,  -116,
      46,  -116,    73,  -116,  -116,  -116,  -116,    74,  -116,    98,
      77,  -116,  -116,    46,     4,    78,  -116,  -116,  -116,    80,
    -116,  -116,    81,  -116,    75,  -116,  -116,  -116,    76,  -116,
    -116,  -116,     6,    43,  -116,  -116,  -116,  -116,    82,    85,
    -116,    -7,    84,    43,  -116,    92,    89,   105,    90,  -116,
     -10,    91,  -116,  -116,  -116,  -116,    86,  -116,  -116,   -10,
    -116
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     3,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     2,     4,     6,     7,     8,     9,    10,    11,    12,
      13,    86,    25,     0,    23,    24,     0,    76,    42,    32,
       1,     5,     0,     0,    31,    19,    78,     0,     0,    88,
      17,     0,     0,     0,    34,    95,    96,    92,     0,    89,
      90,     0,    15,    18,     0,    29,     0,    79,    80,     0,
      74,     0,    38,    74,     0,    35,    36,    39,     0,     0,
      91,    93,    16,    27,     0,    17,    84,    54,    52,    85,
      51,     0,     0,    81,    82,    46,    45,     0,     0,    72,
       0,    73,     0,    37,    21,    20,    22,     0,    87,     0,
      17,    26,    30,     0,     0,     0,    83,    43,    75,     0,
      33,    40,     0,    28,     0,    56,    53,    77,     0,    41,
      94,    55,     0,    47,    66,    63,    65,    64,     0,    59,
      60,     0,     0,    48,    49,    67,     0,     0,     0,    50,
       0,     0,    61,    62,    44,    71,    68,    69,    57,     0,
      70
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Parser::yypgoto_[] =
  {
      -116,  -116,  -116,   108,     1,  -116,    20,  -116,   -77,    -6,
    -116,  -116,    52,  -116,  -116,  -116,  -116,  -116,  -116,  -116,
     -28,  -116,  -116,  -116,  -116,  -116,  -116,  -116,  -115,  -116,
    -116,    14,  -116,  -116,  -116,    -5,  -116,  -116,  -116,   -20,
     -42,    87,  -116,  -116,  -116,  -116,  -116,    47,  -116,  -116,
    -116,  -116,    88,  -116,  -116
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,    10,    11,    12,    62,    53,    54,    14,    97,    63,
      15,    33,    55,   100,    56,    16,    17,    38,    64,    65,
      78,    67,    18,    37,   118,    87,   132,   133,    79,    80,
     103,   116,   122,   128,   129,   130,   131,   141,   146,   147,
      81,    89,    19,    36,    42,    82,    83,    84,    20,    32,
      48,    49,    50,    99,    51
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char Parser::yytable_ninf_ = -59;
  const short int
  Parser::yytable_[] =
  {
        26,    13,    68,    24,   104,    85,    24,   124,   134,    86,
       2,    21,    13,   109,    25,    60,    66,    25,   139,    90,
       1,   125,   126,    68,     2,     3,   114,   127,    45,     4,
       5,    22,    46,     6,    23,    57,     7,    93,     8,   111,
      47,    52,    27,   115,    28,    71,   -58,     9,     2,     2,
      47,    76,    94,    29,    72,    30,    24,    24,    39,    95,
      74,    75,    34,    96,    40,    60,    60,    25,    25,    61,
      61,    35,    77,    77,     2,     3,    52,     2,    41,     4,
       5,    43,     2,     6,    44,    24,     7,    58,     8,   137,
     124,    59,    73,    69,    60,    88,    25,     9,    61,    92,
      98,    52,   101,   107,   125,   126,   108,   105,   110,   111,
     127,   112,   123,   117,   115,   119,   120,   140,   143,    31,
     113,   138,   135,   136,   149,   144,   148,   102,   121,   150,
     106,   142,     0,     0,   145,     0,     0,    70,     0,     0,
       0,     0,     0,   145,     0,     0,     0,     0,     0,     0,
      91
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
         6,     0,    44,    13,    81,     9,    13,     1,   123,    13,
       5,    13,    11,    90,    24,    22,    44,    24,   133,    61,
       1,    15,    16,    65,     5,     6,   103,    21,    23,    10,
      11,    13,    27,    14,    13,    41,    17,    65,    19,    35,
      39,    40,    13,    39,    13,    51,    40,    28,     5,     5,
      49,     7,     6,    13,    53,     0,    13,    13,    36,    13,
      37,    38,    35,    17,    36,    22,    22,    24,    24,    26,
      26,    35,    29,    29,     5,     6,    75,     5,    12,    10,
      11,    39,     5,    14,    36,    13,    17,    36,    19,   131,
       1,     8,    13,    37,    22,    41,    24,    28,    26,    37,
      35,   100,    35,    40,    15,    16,    42,    37,    35,    35,
      21,    13,    36,    35,    39,    35,    35,    25,    13,    11,
     100,    37,    40,    38,    38,    35,    35,    75,   114,   149,
      83,   136,    -1,    -1,   140,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      63
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     1,     5,     6,    10,    11,    14,    17,    19,    28,
      44,    45,    46,    47,    50,    53,    58,    59,    65,    85,
      91,    13,    13,    13,    13,    24,    52,    13,    13,    13,
       0,    46,    92,    54,    35,    35,    86,    66,    60,    36,
      36,    12,    87,    39,    36,    23,    27,    47,    93,    94,
      95,    97,    47,    48,    49,    55,    57,    52,    36,     8,
      22,    26,    47,    52,    61,    62,    63,    64,    83,    37,
      95,    52,    47,    13,    37,    38,     7,    29,    63,    71,
      72,    83,    88,    89,    90,     9,    13,    68,    41,    84,
      83,    84,    37,    63,     6,    13,    17,    51,    35,    96,
      56,    35,    55,    73,    51,    37,    90,    40,    42,    51,
      35,    35,    13,    49,    51,    39,    74,    35,    67,    35,
      35,    74,    75,    36,     1,    15,    16,    21,    76,    77,
      78,    79,    69,    70,    71,    40,    38,    83,    37,    71,
      25,    80,    78,    13,    35,    52,    81,    82,    35,    38,
      82
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
      45,    43,    42,    47,   285,    59,   123,   125,    44,    40,
      41,    91,    93
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    43,    44,    44,    45,    45,    46,    46,    46,    46,
      46,    46,    46,    46,    47,    48,    48,    49,    49,    50,
      51,    51,    51,    52,    52,    54,    53,    56,    55,    57,
      57,    58,    60,    59,    61,    61,    62,    62,    63,    63,
      64,    64,    66,    67,    65,    68,    68,    69,    69,    70,
      70,    71,    71,    72,    73,    72,    75,    74,    76,    76,
      77,    77,    78,    79,    79,    79,    79,    80,    80,    81,
      81,    82,    83,    83,    84,    84,    86,    85,    87,    87,
      88,    88,    89,    89,    90,    90,    92,    91,    93,    93,
      94,    94,    95,    96,    95,    97,    97
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     1,     3,
       1,     1,     1,     1,     1,     0,     7,     0,     4,     1,
       3,     3,     0,     7,     0,     1,     1,     2,     1,     1,
       3,     4,     0,     0,    12,     1,     1,     0,     1,     1,
       2,     1,     1,     3,     0,     4,     0,     6,     0,     1,
       1,     3,     3,     1,     1,     1,     1,     0,     2,     1,
       3,     1,     2,     2,     0,     2,     0,     8,     0,     2,
       0,     1,     1,     2,     1,     1,     0,     7,     0,     1,
       1,     2,     1,     0,     5,     1,     1
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
  "'{'", "'}'", "','", "'('", "')'", "'['", "']'", "$accept", "csl_file",
  "specification_list", "specification", "comment", "comment_list",
  "opt_comment_list", "import_clause", "identifier", "qualified_id",
  "enum_spec", "$@1", "enum_identifier", "$@2", "enum_identifier_list",
  "exception_spec", "struct_spec", "$@3", "opt_record_member_list",
  "record_member_list", "record_member", "field_decl", "nativeclass_spec",
  "$@4", "$@5", "cpp_type", "opt_class_member_list", "class_member_list",
  "class_member", "method_decl", "$@6", "method_remaining_parts", "$@7",
  "opt_param_list", "param_list", "param", "inout", "opt_exception_list",
  "exception_list", "exception", "type_decl", "opt_array",
  "interface_spec", "$@8", "opt_inheritance_decl",
  "opt_interface_member_list", "interface_member_list", "interface_member",
  "component_spec", "$@9", "opt_component_member_list",
  "component_member_list", "component_member", "$@10", "port_kind", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        44,     0,    -1,    45,    -1,     1,    -1,    46,    -1,    45,
      46,    -1,    47,    -1,    50,    -1,    53,    -1,    58,    -1,
      59,    -1,    65,    -1,    85,    -1,    91,    -1,     5,    -1,
      47,    -1,    48,    47,    -1,    -1,    48,    -1,    14,    52,
      35,    -1,    13,    -1,     6,    -1,    17,    -1,    13,    -1,
      24,    -1,    -1,    10,    13,    54,    36,    57,    37,    35,
      -1,    -1,    49,    13,    56,    49,    -1,    55,    -1,    57,
      38,    55,    -1,    11,    13,    35,    -1,    -1,    28,    13,
      60,    36,    61,    37,    35,    -1,    -1,    62,    -1,    63,
      -1,    62,    63,    -1,    47,    -1,    64,    -1,    83,    51,
      35,    -1,    26,    83,    51,    35,    -1,    -1,    -1,    19,
      13,    66,    39,     8,    68,    40,    67,    36,    69,    37,
      35,    -1,    13,    -1,     9,    -1,    -1,    70,    -1,    71,
      -1,    70,    71,    -1,    72,    -1,    63,    -1,    83,    51,
      74,    -1,    -1,    29,    73,    51,    74,    -1,    -1,    39,
      75,    76,    40,    80,    35,    -1,    -1,    77,    -1,    78,
      -1,    77,    38,    78,    -1,    79,    83,    13,    -1,    15,
      -1,    21,    -1,    16,    -1,     1,    -1,    -1,    25,    81,
      -1,    82,    -1,    81,    38,    82,    -1,    52,    -1,    22,
      84,    -1,    52,    84,    -1,    -1,    41,    42,    -1,    -1,
      17,    13,    86,    87,    36,    88,    37,    35,    -1,    -1,
      12,    52,    -1,    -1,    89,    -1,    90,    -1,    89,    90,
      -1,     7,    -1,    71,    -1,    -1,     6,    13,    92,    36,
      93,    37,    35,    -1,    -1,    94,    -1,    95,    -1,    94,
      95,    -1,    47,    -1,    -1,    97,    52,    96,    13,    35,
      -1,    23,    -1,    27,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     7,     9,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    35,    36,    38,
      42,    44,    46,    48,    50,    52,    53,    61,    62,    67,
      69,    73,    77,    78,    86,    87,    89,    91,    94,    96,
      98,   102,   107,   108,   109,   122,   124,   126,   127,   129,
     131,   134,   136,   138,   142,   143,   148,   149,   156,   157,
     159,   161,   165,   169,   171,   173,   175,   177,   178,   181,
     183,   187,   189,   192,   195,   196,   199,   200,   209,   210,
     213,   214,   216,   218,   221,   223,   225,   226,   234,   235,
     237,   239,   242,   244,   245,   251,   253
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   109,   109,   110,   124,   125,   129,   130,   131,   132,
     133,   134,   135,   136,   140,   144,   145,   148,   150,   154,
     159,   160,   161,   165,   166,   170,   170,   175,   175,   179,
     180,   184,   188,   188,   192,   194,   198,   199,   203,   204,
     208,   209,   213,   214,   213,   219,   220,   223,   225,   229,
     230,   234,   235,   239,   240,   240,   244,   244,   248,   250,
     254,   255,   259,   264,   265,   266,   267,   271,   273,   277,
     278,   282,   286,   287,   291,   292,   296,   296,   300,   302,
     305,   307,   311,   312,   316,   317,   321,   321,   325,   327,
     331,   332,   336,   337,   337,   342,   343
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
      39,    40,    32,    31,    38,    30,     2,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    35,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    41,     2,    42,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    36,     2,    37,     2,     2,     2,     2,
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
  const int Parser::yylast_ = 150;
  const int Parser::yynnts_ = 55;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 30;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 43;

  const unsigned int Parser::yyuser_token_number_max_ = 285;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


} } // co::csl





void co::csl::Parser::error( const location_type&, const std::string& )
{
	// ignore
}

