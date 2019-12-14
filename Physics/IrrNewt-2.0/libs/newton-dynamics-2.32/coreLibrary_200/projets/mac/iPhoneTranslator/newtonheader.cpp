
/*  A Bison parser, made from c:\newton_200\newtonsdk\iphonetranslator\newtonheader.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse Newtonparse
#define yylex Newtonlex
#define yyerror Newtonerror
#define yylval Newtonlval
#define yychar Newtonchar
#define yydebug Newtondebug
#define yynerrs Newtonnerrs
#define	INT	258
#define	CHAR	259
#define	VOID	260
#define	CONST	261
#define	SHORT	262
#define	UNSIGNED	263
#define	FLOAT	264
#define	DOUBLE	265
#define	STRUCT	266
#define	UNION	267
#define	TYPEDEF	268
#define	IFDEF	269
#define	IFNDEF	270
#define	ELSE	271
#define	ENDIF	272
#define	DEFINE	273
#define	INCLUDE	274
#define	NUMERIC_CONSTANT	275
#define	LITERAL_IDENTIFIER	276
#define	QUOTED_CONSTANT	277
#define	NEG	278


	#include <stdafx.h>
	#include "main.h"


	#define YYDEBUG 1
	#define YYERROR_VERBOSE
	#define YYSTYPE LexicalData

	static	int tab;
	void GenerateTabs (char* tabs)
	{
		tabs[0] = 0;
		for (int i = 0; i < tab; i ++) {
			strcat (tabs, "\t");   
		}
	}

	extern char yytext[];
	//extern int column;
	int yyerror(char *s)
	{
	//	fflush(stdout);
	//	printf("\n%*s\n%*s\n", column, "^", column, s);
		return 0;
	}

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		143
#define	YYFLAG		-32768
#define	YYNTBASE	39

#define YYTRANSLATE(x) ((unsigned)(x) <= 278 ? yytranslate[x] : 66)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    33,
    34,    26,    24,    38,    25,     2,    27,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    35,    29,
    23,    30,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    36,     2,    37,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    31,     2,    32,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    28
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     5,     8,    10,    13,    15,    17,    19,    21,    23,
    25,    27,    29,    32,    36,    40,    43,    48,    53,    58,
    65,    72,    79,    84,    87,    91,    95,   102,   104,   106,
   114,   116,   120,   124,   127,   129,   132,   136,   143,   153,
   155,   157,   160,   164,   168,   173,   175,   177,   179,   181,
   183,   186,   189,   192,   195,   198,   200,   203,   206,   212,
   217,   224,   226,   229,   232,   236,   238,   245,   253,   255,
   258,   261,   265,   270
};

static const short yyrhs[] = {    15,
    21,    40,    17,     0,    41,    46,     0,    42,     0,    41,
    42,     0,    43,     0,    44,     0,    49,     0,    51,     0,
    58,     0,    63,     0,    46,     0,    45,     0,    18,    21,
     0,    18,    50,    20,     0,    18,    21,    22,     0,    19,
    22,     0,    19,    29,    21,    30,     0,    48,    21,    47,
    17,     0,    48,    21,    46,    17,     0,    48,    21,    47,
    16,    46,    17,     0,    48,    21,    47,    16,    47,    17,
     0,    48,    21,    21,    22,    31,    17,     0,    48,    21,
    32,    17,     0,    18,    21,     0,    18,     9,     9,     0,
    18,     9,    10,     0,    18,    21,    21,    33,    21,    34,
     0,    14,     0,    15,     0,    13,    11,    50,    31,    32,
    50,    35,     0,    21,     0,    52,    53,    57,     0,    11,
    21,    31,     0,    12,    31,     0,    54,     0,    53,    54,
     0,    55,    50,    35,     0,    55,    50,    36,    20,    37,
    35,     0,    55,    50,    36,    20,    37,    36,    20,    37,
    35,     0,    51,     0,    56,     0,    56,    26,     0,    56,
    26,     6,     0,    56,    26,    26,     0,    56,    26,    26,
     6,     0,     3,     0,     4,     0,     5,     0,     9,     0,
     8,     0,     7,     3,     0,     8,     7,     0,     6,     5,
     0,     6,     3,     0,     6,     9,     0,    50,     0,     6,
    50,     0,    32,    35,     0,    59,    33,    60,    34,    35,
     0,    59,    33,    34,    35,     0,    13,    55,    33,    26,
    50,    34,     0,    61,     0,    60,    61,     0,    55,    50,
     0,    55,    50,    38,     0,    50,     0,    50,    55,    62,
    33,    34,    35,     0,    50,    55,    62,    33,    64,    34,
    35,     0,    65,     0,    64,    65,     0,    55,    50,     0,
    55,    50,    38,     0,    55,    50,    36,    37,     0,    55,
    50,    36,    37,    38,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    63,    68,    72,    73,    77,    81,    85,    89,    93,    97,
   101,   102,   106,   109,   113,   117,   118,   122,   123,   124,
   125,   126,   127,   131,   132,   133,   134,   138,   139,   144,
   152,   160,   167,   174,   184,   188,   195,   201,   207,   213,
   220,   224,   228,   233,   238,   247,   251,   255,   259,   263,
   267,   271,   275,   279,   283,   287,   291,   299,   311,   315,
   322,   330,   334,   341,   345,   353,   364,   373,   385,   389,
   396,   405,   414,   418
};

static const char * const yytname[] = {   "$","error","$undefined.","INT","CHAR",
"VOID","CONST","SHORT","UNSIGNED","FLOAT","DOUBLE","STRUCT","UNION","TYPEDEF",
"IFDEF","IFNDEF","ELSE","ENDIF","DEFINE","INCLUDE","NUMERIC_CONSTANT","LITERAL_IDENTIFIER",
"QUOTED_CONSTANT","'='","'+'","'-'","'*'","'/'","NEG","'<'","'>'","'{'","'}'",
"'('","')'","';'","'['","']'","','","NewtonHeader","CDeclareAndClassDeclare",
"StatementList","Statement","HeaderInfo","DefineConstant","Include","IfDefStatement",
"EmptyDefine","IfDef","TypeDefStructures","literalIndentifer","StructuresDeclaration",
"StructuresDeclarationInit","DataInstanceList","DataInstance","DataType","DataInstace",
"StructuresDeclarationEnd","FunctionCallBackDeclaration","FunctionCallBackDeclarationName",
"FunctionCallBackDeclarationParamsList","FunctionCallBackDeclarationParam","ClassMethodName",
"ClassMethod","functionArgumentList","functionArgument",""
};
#endif

static const short yyr1[] = {     0,
    39,    40,    41,    41,    42,    42,    42,    42,    42,    42,
    42,    42,    43,    44,    44,    45,    45,    46,    46,    46,
    46,    46,    46,    47,    47,    47,    47,    48,    48,    49,
    50,    51,    52,    52,    53,    53,    54,    54,    54,    54,
    55,    55,    55,    55,    55,    56,    56,    56,    56,    56,
    56,    56,    56,    56,    56,    56,    56,    57,    58,    58,
    59,    60,    60,    61,    61,    62,    63,    63,    64,    64,
    65,    65,    65,    65
};

static const short yyr2[] = {     0,
     4,     2,     1,     2,     1,     1,     1,     1,     1,     1,
     1,     1,     2,     3,     3,     2,     4,     4,     4,     6,
     6,     6,     4,     2,     3,     3,     6,     1,     1,     7,
     1,     3,     3,     2,     1,     2,     3,     6,     9,     1,
     1,     2,     3,     3,     4,     1,     1,     1,     1,     1,
     2,     2,     2,     2,     2,     1,     2,     2,     5,     4,
     6,     1,     2,     2,     3,     1,     6,     7,     1,     2,
     2,     3,     4,     5
};

static const short yydefact[] = {     0,
     0,     0,     0,     0,     0,    28,    29,     0,     0,    31,
     0,     0,     3,     5,     6,    12,    11,     0,     7,     0,
     8,     0,     9,     0,    10,     0,    34,    46,    47,    48,
     0,     0,    50,    49,     0,    56,     0,    41,    13,     0,
    16,     0,     1,     4,    11,     0,     0,    40,     0,    35,
     0,     0,    33,    54,    53,    55,    57,    51,    52,     0,
     0,    42,    15,    14,     0,     0,     0,     0,     0,     0,
    66,     0,     0,    36,    32,     0,     0,     0,     0,    62,
     0,     0,    43,    44,    17,     0,    24,     0,    23,    19,
     0,    18,     0,    58,    37,     0,    60,    64,     0,    63,
     0,     0,    45,    25,    26,     0,     0,     0,     0,     0,
     0,     0,    69,     0,    65,    59,     0,    61,     0,    22,
    20,    21,    67,    71,     0,    70,     0,    30,     0,     0,
    72,    68,    38,     0,    27,    73,     0,    74,     0,    39,
     0,     0,     0
};

static const short yydefgoto[] = {   141,
    11,    12,    13,    14,    15,    16,    17,    70,    18,    19,
    36,    21,    22,    49,    50,    51,    38,    75,    23,    24,
    79,    80,    72,    25,   112,   113
};

static const short yypact[] = {     3,
   -16,   142,     7,     4,   141,-32768,-32768,    26,   -18,-32768,
    61,   142,-32768,-32768,-32768,-32768,-32768,    45,-32768,    89,
-32768,   130,-32768,    46,-32768,    60,-32768,-32768,-32768,-32768,
    49,    98,    95,-32768,    82,-32768,    71,    80,    31,    85,
-32768,    86,-32768,-32768,    94,    -6,    82,-32768,   111,-32768,
    82,    16,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    81,
    99,     1,-32768,-32768,    91,    25,   102,   109,   110,    15,
-32768,    96,    93,-32768,-32768,    32,   105,    82,    35,-32768,
   126,    82,-32768,   124,-32768,    62,   138,   100,-32768,-32768,
    -1,-32768,    56,-32768,-32768,   144,-32768,   127,   131,-32768,
    82,   133,-32768,-32768,-32768,   135,   152,   153,   154,   137,
    82,    79,-32768,   136,-32768,-32768,   139,-32768,   155,-32768,
-32768,-32768,-32768,    19,   140,-32768,    38,-32768,   143,   145,
-32768,-32768,-32768,   158,-32768,   146,   148,-32768,   151,-32768,
   179,   180,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,   169,-32768,-32768,-32768,   -10,    92,-32768,-32768,
    -2,   -19,-32768,-32768,   147,    -4,-32768,-32768,-32768,-32768,
-32768,   108,-32768,-32768,-32768,    76
};


#define	YYLAST		196


static const short yytable[] = {    20,
    37,    45,    48,    41,     2,    40,    83,     6,     7,    20,
    42,    66,     6,     7,    67,    47,    66,     1,    28,    29,
    30,    31,    32,    33,    34,    68,    84,    26,    57,    48,
    91,    92,    60,    86,    27,    69,    10,    28,    29,    30,
    31,    32,    33,    34,    71,    87,    39,    78,    76,    77,
   -31,    54,    63,    55,   130,    10,   131,    56,    28,    29,
    30,    31,    32,    33,    34,    46,    95,    96,    99,    10,
   104,   105,   133,   134,    78,    98,    10,    43,    52,   102,
   108,    28,    29,    30,    31,    32,    33,    34,   111,   110,
    53,    28,    29,    30,    31,    32,    33,    34,   117,    10,
    58,    59,    10,    61,    64,    62,    65,   111,   124,    10,
    -2,    81,   125,    28,    29,    30,    31,    32,    33,    34,
    85,     3,     4,    88,    82,    89,    90,    94,    93,   103,
   107,    10,    28,    29,    30,    31,    32,    33,    34,    97,
     3,     4,    73,    28,    29,    30,    31,    32,    33,    34,
    10,    35,     3,     4,     5,     6,     7,   101,   106,     8,
     9,    10,    10,   114,   115,   116,   118,   119,   120,   121,
   122,   123,   127,   128,   132,   129,   135,   137,   142,   143,
    44,   136,   109,   138,   139,   140,   100,   126,     0,     0,
     0,     0,     0,     0,     0,    74
};

static const short yycheck[] = {     2,
     5,    12,    22,    22,    21,     8,     6,    14,    15,    12,
    29,    18,    14,    15,    21,    20,    18,    15,     3,     4,
     5,     6,     7,     8,     9,    32,    26,    21,    31,    49,
    16,    17,    35,     9,    31,    46,    21,     3,     4,     5,
     6,     7,     8,     9,    47,    21,    21,    52,    51,    34,
    20,     3,    22,     5,    36,    21,    38,     9,     3,     4,
     5,     6,     7,     8,     9,    21,    35,    36,    34,    21,
     9,    10,    35,    36,    79,    78,    21,    17,    33,    82,
    91,     3,     4,     5,     6,     7,     8,     9,    93,    34,
    31,     3,     4,     5,     6,     7,     8,     9,   101,    21,
     3,     7,    21,    33,    20,    26,    21,   112,   111,    21,
    17,    31,    34,     3,     4,     5,     6,     7,     8,     9,
    30,    11,    12,    22,    26,    17,    17,    35,    33,     6,
    31,    21,     3,     4,     5,     6,     7,     8,     9,    35,
    11,    12,    32,     3,     4,     5,     6,     7,     8,     9,
    21,    11,    11,    12,    13,    14,    15,    32,    21,    18,
    19,    21,    21,    20,    38,    35,    34,    33,    17,    17,
    17,    35,    37,    35,    35,    21,    34,    20,     0,     0,
    12,    37,    91,    38,    37,    35,    79,   112,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    49
};

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */



/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
   char *f = from;
   char *t = to;
   int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
   char *f = from;
   char *t = to;
   int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
   int yystate;
   int yyn;
   short *yyssp;
   YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = (short)yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 5:
{
//			PrintHeader ($1.m_literal);
		;
    break;}
case 6:
{
			PrintDefine (yyvsp[0].m_literal);
		;
    break;}
case 7:
{
			PrintStrutureDefine (yyvsp[0].m_literal);
		;
    break;}
case 8:
{
			PrintDataStrutures (yyvsp[0].m_literal);
		;
    break;}
case 9:
{
			PrintFunctionCallbacks (yyvsp[0].m_literal);		
		;
    break;}
case 10:
{
			PrintClassMethod (yyvsp[0].m_literal);
		;
    break;}
case 14:
{
		sprintf (yyval.m_literal, "#define %s\t%s\n", yyvsp[-1].m_literal, yyvsp[0].m_literal);
	  ;
    break;}
case 30:
{
		sprintf (yyval.m_literal, "typedef struct i%s{} i%s;\n", yyvsp[-4].m_literal, yyvsp[-1].m_literal);
	  ;
    break;}
case 31:
{
			yyval = yyvsp[0];
		;
    break;}
case 32:
{
			sprintf (yyval.m_literal, "%s%s%s\n", yyvsp[-2].m_literal, yyvsp[-1].m_literal, yyvsp[0].m_literal);
		;
    break;}
case 33:
{
			char tabs[256];
			GenerateTabs (tabs);
			sprintf (yyval.m_literal, "%sstruct i%s {\n", tabs, yyvsp[-1].m_literal);
			tab ++;
		;
    break;}
case 34:
{
			char tabs[256];
			GenerateTabs (tabs);
			sprintf (yyval.m_literal, "%sunion {\n", tabs);
			tab ++;
		;
    break;}
case 35:
{
			yyval = yyvsp[0];
		;
    break;}
case 36:
{
			sprintf (yyval.m_literal, "%s%s", yyvsp[-1].m_literal, yyvsp[0].m_literal);
		;
    break;}
case 37:
{
			char tabs[256];
			GenerateTabs (tabs);
			sprintf (yyval.m_literal, "%s%s %s;\n", tabs, yyvsp[-2].m_literal, yyvsp[-1].m_literal);
		;
    break;}
case 38:
{
			char tabs[256];
			GenerateTabs (tabs);
			sprintf (yyval.m_literal, "%s%s %s[%s];\n", tabs, yyvsp[-5].m_literal, yyvsp[-4].m_literal, yyvsp[-2].m_literal);
		;
    break;}
case 39:
{		
			char tabs[256];
			GenerateTabs (tabs);
			sprintf (yyval.m_literal, "%s%s %s[%s][%s];\n", tabs, yyvsp[-8].m_literal, yyvsp[-7].m_literal, yyvsp[-5].m_literal, yyvsp[-2].m_literal);		
		;
    break;}
case 40:
{
			sprintf (yyval.m_literal, "%s", yyvsp[0].m_literal);
		;
    break;}
case 41:
{
			yyval = yyvsp[0];
		;
    break;}
case 42:
{
			sprintf (yyval.m_literal, "%s*", yyvsp[-1].m_literal);
		;
    break;}
case 43:
{
			//sprintf ($$.m_literal, "%s*", $1.m_literal);
			sprintf (yyval.m_literal, "%s* const", yyvsp[-2].m_literal);
		;
    break;}
case 44:
{
			sprintf (yyval.m_literal, "%s**", yyvsp[-2].m_literal);
		;
    break;}
case 45:
{
			sprintf (yyval.m_literal, "%s** const", yyvsp[-3].m_literal);
			//sprintf ($$.m_literal, "%s**", $1.m_literal);
		;
    break;}
case 46:
{
			sprintf (yyval.m_literal, "int");						
		;
    break;}
case 47:
{
			sprintf (yyval.m_literal, "char");			
		;
    break;}
case 48:
{
			sprintf (yyval.m_literal, "void");
		;
    break;}
case 49:
{
			sprintf (yyval.m_literal, "float");
		;
    break;}
case 50:
{
			sprintf (yyval.m_literal, "unsigned");
		;
    break;}
case 51:
{
			sprintf (yyval.m_literal, "short int");
		;
    break;}
case 52:
{
			sprintf (yyval.m_literal, "unsigned short");
		;
    break;}
case 53:
{
			sprintf (yyval.m_literal, "const void");
		;
    break;}
case 54:
{
			sprintf (yyval.m_literal, "const int");
		;
    break;}
case 55:
{
			sprintf (yyval.m_literal, "const float");
		;
    break;}
case 56:
{
			sprintf (yyval.m_literal, "struct i%s", yyvsp[0].m_literal);
		;
    break;}
case 57:
{
			sprintf (yyval.m_literal, "%s struct i%s", yyvsp[-1].m_literal, yyvsp[0].m_literal);
		;
    break;}
case 58:
{
			char tabs[256];
			tab --;
			GenerateTabs (tabs);
			sprintf (yyval.m_literal, "%s};\n", tabs);
		;
    break;}
case 59:
{
		sprintf (yyval.m_literal, "%s (%s);\n", yyvsp[-4].m_literal, yyvsp[-2].m_literal);
	  ;
    break;}
case 60:
{
		sprintf (yyval.m_literal, "%s ();\n", yyvsp[-3].m_literal);
	  ;
    break;}
case 61:
{
		SaveCallback (yyvsp[-1].m_literal);
		sprintf (yyval.m_literal, "typedef %s (*i%s)", yyvsp[-4].m_literal, yyvsp[-1].m_literal);
	  ;
    break;}
case 62:
{
			yyval = yyvsp[0];
		;
    break;}
case 63:
{
			sprintf (yyval.m_literal, "%s%s", yyvsp[-1].m_literal, yyvsp[0].m_literal);
		;
    break;}
case 64:
{
		sprintf (yyval.m_literal, "%s %s", yyvsp[-1].m_literal, yyvsp[0].m_literal);
	  ;
    break;}
case 65:
{
		sprintf (yyval.m_literal, "%s %s, ", yyvsp[-2].m_literal, yyvsp[-1].m_literal);
	  ;
    break;}
case 66:
{
			if (!strncmp (yyvsp[0].m_literal, "Newton", 6)) {
				sprintf (yyval.m_literal, "%s", yyvsp[0].m_literal + 6);
			} else {
				yyval = yyvsp[0];
			}	
		;
    break;}
case 67:
{
			const char* callback = IsCallback (yyvsp[-4].m_literal);
			if (callback) {
				sprintf (yyval.m_literal, "-(i%s) %s;\n", callback, yyvsp[-3].m_literal);
			} else {
				sprintf (yyval.m_literal, "-(%s) %s;\n", yyvsp[-4].m_literal, yyvsp[-3].m_literal);
			}
		;
    break;}
case 68:
{
			const char* callback = IsCallback (yyvsp[-5].m_literal);
			if (callback) {
				sprintf (yyval.m_literal, "-(i%s) %s%s;\n", callback, yyvsp[-4].m_literal, yyvsp[-2].m_literal);
			} else {
				sprintf (yyval.m_literal, "-(%s) %s%s;\n", yyvsp[-5].m_literal, yyvsp[-4].m_literal, yyvsp[-2].m_literal);
			}
		;
    break;}
case 69:
{
			yyval = yyvsp[0];
		;
    break;}
case 70:
{
			sprintf (yyval.m_literal, "%s%s", yyvsp[-1].m_literal, yyvsp[0].m_literal);
		;
    break;}
case 71:
{
		const char* callback = IsCallback (yyvsp[-1].m_literal);
		if (callback) {
			sprintf (yyval.m_literal, ": (i%s) %s", callback, yyvsp[0].m_literal);
		} else {
			sprintf (yyval.m_literal, ": (%s) %s", yyvsp[-1].m_literal, yyvsp[0].m_literal);
		}
	 ;
    break;}
case 72:
{
		const char* callback = IsCallback (yyvsp[-2].m_literal);
		if (callback) {
			sprintf (yyval.m_literal, ": (i%s) %s", callback, yyvsp[-1].m_literal);
		} else {
			sprintf (yyval.m_literal, ": (%s) %s", yyvsp[-2].m_literal, yyvsp[-1].m_literal);
		}
	 ;
    break;}
case 73:
{
		sprintf (yyval.m_literal, ": (%s*) %s", yyvsp[-3].m_literal, yyvsp[-2].m_literal);
	 ;
    break;}
case 74:
{
		sprintf (yyval.m_literal, ": (%s*) %s", yyvsp[-4].m_literal, yyvsp[-3].m_literal);
	 ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif


  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif
  yystate = yyn;
  goto yynewstate;
}




