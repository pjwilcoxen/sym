/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20170430

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#define YYPREFIX "yy"

#define YYPURE 0

#line 1 "parse.y"

/*--------------------------------------------------------------------*
 *  PARSE.Y 4.00
 *  9 Apr 90 (PJW)
 *
 *  Parser specification for symbolic math program.  This file must
 *  be processed by yacc or bison.
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include "str.h"
#include "sym.h"
#include "nodes.h"
#include "declare.h"
#include "eqns.h"

#define YYSTYPE Node*

int yylex();
void yyerror(char*);

#line 43 "parse.c"

#if ! defined(YYSTYPE) && ! defined(YYSTYPE_IS_DECLARED)
/* Default: YYSTYPE is the semantic value type. */
typedef int YYSTYPE;
# define YYSTYPE_IS_DECLARED 1
#endif

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define NAME 257
#define NUM 258
#define NEG 259
#define LOG 260
#define EXP 261
#define SUM 262
#define PROD 263
#define STRING 264
#define BADNUM 265
#define SET 266
#define VAR 267
#define PAR 268
#define EQU 269
#define LEAD 270
#define LAG 271
#define NEXT 272
#define TIME 273
#define FIRST 274
#define LAST 275
#define YYERRCODE 256
typedef short YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    0,    1,    1,    1,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    4,    4,    7,    7,
    7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
    8,    8,    9,    9,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
   13,   13,   12,    5,    5,    5,    6,    6,   10,   10,
   10,   10,   10,   11,   11,   11,   11,   11,   11,   11,
   11,   11,   11,   11,   11,   11,   18,   18,   18,   18,
   14,   14,   16,   16,   17,   17,   17,   17,   15,   15,
};
static const YYINT yylen[] = {                            2,
    1,    3,    0,    1,    1,    2,    3,    3,    4,    4,
    5,    6,    6,    7,    7,    1,    1,    1,    5,    6,
    4,    5,    7,    8,    7,    8,    8,    9,    8,    9,
    1,    1,    1,    1,    3,    4,    4,    5,    4,    5,
    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
    3,    2,    2,    1,    3,    3,    1,    3,    1,    1,
    1,    1,    1,    3,    3,    3,    3,    3,    2,    3,
    1,    1,    1,    1,    1,    1,    1,    4,    3,    6,
    4,    4,    4,    4,    4,    4,    4,    4,    6,    6,
};
static const YYINT yydefred[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,   18,   17,    0,
    0,    0,    0,   63,   61,   62,    0,    0,    0,    0,
    1,    4,    5,    0,    0,   16,    0,    0,    0,    0,
   71,   72,   73,   74,   75,    0,    0,    0,    0,    0,
    0,    0,   32,   33,   34,    0,    0,   52,    0,    0,
    0,    0,   76,   69,    0,    0,    0,    0,   53,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   59,
   60,    0,   79,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   51,   70,    2,    0,
    0,    0,    0,    8,    0,   58,    0,    0,    0,    0,
    0,   68,    0,    0,    0,    0,   81,   82,    0,    0,
    0,    0,    0,    0,   86,   85,   84,   83,   88,   87,
    9,   54,    0,    0,    0,   10,    0,   37,    0,    0,
    0,    0,    0,    0,   22,    0,    0,    0,    0,    0,
    0,   55,   56,   38,    0,   41,    0,   45,    0,   80,
   89,   90,    0,    0,    0,   20,    0,    0,   13,    0,
   42,   46,    0,   49,    0,    0,    0,    0,   14,   15,
   50,    0,    0,   24,   26,   28,   30,
};
static const YYINT yydgoto[] = {                         20,
   21,   22,   23,   24,   94,  122,   26,   46,   47,   27,
   28,   29,   30,   31,   32,   33,   34,   35,
};
static const YYINT yysindex[] = {                       -40,
  -27,    0,  -30,  -29,  -24,  -19, -102,    0,    0, -226,
   -7,   13,   17,    0,    0,    0,  130, -216,  130,   42,
    0,    0,    0, -142,   70,    0,   79,  401,  130,  -21,
    0,    0,    0,    0,    0,  -11, -128,  130,  130, -117,
 -113,   85,    0,    0,    0,  107,   89,    0, -225, -254,
 -225,  -27,    0,    0,  102,  161,  -40,   57,    0,  -11,
  130,  130,  130,  130,  130,  130,  487,  494,  130,    0,
    0,  110,    0,  418,  425,  115,  117, -255,  -11, -119,
  121,  125,  135,  140,  144,  148,    0,    0,    0,   92,
  -11,  -11,  -11,    0,  -87,    0,   69,   63,   63,   97,
   97,    0,  130,  130,  500,  159,    0,    0,  130,  130,
  -39,  157,  158,  160,    0,    0,    0,    0,    0,    0,
    0,    0,  164,   73,  108,    0,   92,    0,   88,   96,
  130,  -50,  432,  439,    0,  169,  170,  -11,  -52,  -11,
   65,    0,    0,    0,   92,    0,   92,    0,  122,    0,
    0,    0,  -11,  -11,  173,    0,  183,   92,    0,  -26,
    0,    0,   92,    0,  202,  203,  -16,   -8,    0,    0,
    0,   -4,   -3,    0,    0,    0,    0,
};
static const YYINT yyrindex[] = {                        20,
  460,  466,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   49,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  217,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    1,    0,    0,    0,    0,   20,   23,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   25,
    0,    0,    0,    0,   26,    0,   27,   35,   61,   22,
   30,    0,    0,    0,    0,    9,    0,    0,    0,    0,
   15,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   28,    0,   29,   34,
    0,    0,    0,    0,    0,    0,    0,    0,   39,    0,
   40,    0,    0,    0,   58,    0,   59,    0,   66,    0,
    0,    0,    0,    0,    0,    0,    0,  109,    0,   26,
    0,    0,  119,    0,    0,    0,  127,  134,    0,    0,
    0,  136,  137,    0,    0,    0,    0,
};
static const YYINT yygindex[] = {                         0,
  211,    0,    0,    0,  284,  347,    0,  191,    0,  131,
  389,  240,    0,    0,    0,  222,  -37,  -13,
};
#define YYTABLESIZE 594
static const YYINT yytable[] = {                         19,
   77,  111,   52,  136,   17,  137,   18,   37,   78,   38,
   39,   81,   36,   85,   21,   40,   12,   43,   19,    3,
   41,   66,    6,   17,    7,   54,   35,   36,   39,   67,
   48,   52,   49,   43,   64,   82,   84,   86,   19,   11,
   55,   77,   77,   77,   11,   77,   13,   77,   57,   78,
   78,   78,   50,   78,   31,   78,   51,   40,   44,   77,
   65,   77,   66,   66,   66,   47,   66,   78,   66,   78,
   67,   67,   67,   21,   67,   64,   67,   64,    3,   64,
   66,    6,   66,    7,   54,   35,   36,   39,   67,   57,
   67,   77,   43,   64,   77,   64,   91,   19,   11,   78,
   57,   65,   78,   65,   64,   65,   57,   57,   12,   65,
   64,   62,   66,   63,   58,   65,   40,   44,   48,   65,
   67,   65,   60,   77,   47,   64,   23,   59,   73,   64,
   62,   78,   63,   25,   65,   27,   29,   64,   62,   76,
   63,   57,   65,   77,   66,   78,   79,   93,   87,   80,
  106,   65,   67,  114,   42,   93,   66,   64,  109,   93,
  110,  115,   66,   64,   62,  116,   63,   12,   65,   19,
   43,   44,   45,   57,   17,  117,  126,   48,   93,   92,
  118,   66,   93,   65,  119,   23,   93,   92,  120,   66,
   66,   92,   25,  132,   27,   29,  138,  142,  139,  140,
  143,   88,   64,   62,  141,   63,  150,   65,  153,  154,
   92,  156,   93,  167,   92,   66,    1,    2,   92,    3,
    4,    5,    6,  168,  135,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,    1,    2,  170,    3,    4,
    5,    6,  172,  173,   92,   70,   71,  174,   11,   12,
   13,   14,   15,   16,   66,  175,   31,   77,   77,  176,
  177,   14,   15,   16,   77,   78,   78,   89,  112,   69,
  157,   83,   78,   77,   77,   77,    0,    0,   66,   66,
    0,   78,   78,   78,    0,   66,   67,   67,    0,    0,
    0,   64,   64,   67,   66,   66,   66,    0,   64,    0,
    0,    0,   67,   67,   67,    0,    0,   64,   64,   64,
    0,    0,   57,   70,   71,    0,    0,   65,   65,    0,
   90,   70,   71,    0,   65,   70,   71,    0,  158,   14,
   15,   16,  127,   65,   65,   65,    0,   14,   15,   16,
    0,   14,   15,   16,   70,   71,   25,    0,   70,   71,
    0,  145,   70,   71,    0,    0,    0,    0,    0,  147,
   14,   15,   16,    0,   14,   15,   16,    0,   14,   15,
   16,    0,    0,  121,    0,    0,   25,    0,   70,   71,
  128,    0,   72,    0,    0,  163,   52,   53,    0,    3,
    4,    5,    6,    0,   14,   15,   16,    0,    0,   11,
   12,   13,    0,   25,   95,   54,   96,   56,    0,    0,
  144,    0,  146,  148,    0,    0,    0,   67,   68,    0,
    0,    0,    0,    0,  159,  113,   74,   75,  161,    0,
  162,    0,  164,    0,    0,    0,    0,  123,  124,  125,
    0,  169,   64,   62,    0,   63,  171,   65,    0,   97,
   98,   99,  100,  101,  102,    0,    0,  105,  107,   64,
   62,   61,   63,    0,   65,  108,   64,   62,    0,   63,
    0,   65,  151,   64,   62,    0,   63,    0,   65,  152,
   64,   62,    0,   63,  155,   65,    0,  160,    0,    0,
    0,  129,  130,    0,   66,    0,    0,  133,  134,  165,
  166,   77,   77,   59,   77,    0,   77,   76,   76,   60,
   76,   66,   76,    0,    0,    0,    0,   59,   66,  149,
   77,    0,    0,   60,    0,   66,   76,    0,   64,   62,
    0,   63,   66,   65,    0,   64,   62,    0,   63,    0,
   65,   64,   62,    0,   63,    0,   65,  103,    0,    0,
    0,    0,    0,   77,  104,    0,    0,    0,    0,   76,
  131,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   66,    0,    0,    0,    0,    0,    0,   66,    0,    0,
    0,    0,    0,   66,
};
static const YYINT yycheck[] = {                         40,
    0,  257,  257,   43,   45,   45,   47,   35,    0,   40,
   40,   49,   40,   51,    0,   40,  271,  273,   40,    0,
   40,    0,    0,   45,    0,    0,    0,    0,    0,    0,
  257,  257,   40,    0,    0,   49,   50,   51,    0,    0,
  257,   41,   42,   43,  270,   45,  272,   47,    0,   41,
   42,   43,   40,   45,   40,   47,   40,    0,    0,   59,
    0,   61,   41,   42,   43,    0,   45,   59,   47,   61,
   41,   42,   43,   59,   45,   41,   47,   43,   59,   45,
   59,   59,   61,   59,   59,   59,   59,   59,   59,   41,
   61,   91,   59,   59,   94,   61,   40,   59,   59,   91,
   59,   41,   94,   43,   42,   45,   58,   59,    0,   47,
   42,   43,   91,   45,  257,   47,   59,   59,    0,   59,
   91,   61,   44,  123,   59,   91,    0,   58,  257,   42,
   43,  123,   45,    0,   47,    0,    0,   42,   43,  257,
   45,   93,   47,  257,  123,   61,   40,   91,   47,   61,
   41,   91,  123,  273,  257,   91,   94,  123,   44,   91,
   44,   41,   94,   42,   43,   41,   45,   59,   47,   40,
  273,  274,  275,  125,   45,   41,  264,   59,   91,  123,
   41,   94,   91,  123,   41,   59,   91,  123,   41,   94,
   94,  123,   59,   35,   59,   59,   40,  125,   41,   40,
   93,   41,   42,   43,   41,   45,  257,   47,   40,   40,
  123,  264,   91,   41,  123,   94,  257,  258,  123,  260,
  261,  262,  263,   41,  264,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  257,  258,  264,  260,  261,
  262,  263,   41,   41,  123,  257,  258,  264,  270,  271,
  272,  273,  274,  275,   94,  264,   40,  257,  258,  264,
  264,  273,  274,  275,  264,  257,  258,   57,   78,   30,
  140,   50,  264,  273,  274,  275,   -1,   -1,  257,  258,
   -1,  273,  274,  275,   -1,  264,  257,  258,   -1,   -1,
   -1,  257,  258,  264,  273,  274,  275,   -1,  264,   -1,
   -1,   -1,  273,  274,  275,   -1,   -1,  273,  274,  275,
   -1,   -1,  264,  257,  258,   -1,   -1,  257,  258,   -1,
  264,  257,  258,   -1,  264,  257,  258,   -1,  264,  273,
  274,  275,  264,  273,  274,  275,   -1,  273,  274,  275,
   -1,  273,  274,  275,  257,  258,    0,   -1,  257,  258,
   -1,  264,  257,  258,   -1,   -1,   -1,   -1,   -1,  264,
  273,  274,  275,   -1,  273,  274,  275,   -1,  273,  274,
  275,   -1,   -1,   90,   -1,   -1,   30,   -1,  257,  258,
   97,   -1,   36,   -1,   -1,  264,  257,  258,   -1,  260,
  261,  262,  263,   -1,  273,  274,  275,   -1,   -1,  270,
  271,  272,   -1,   57,   58,   17,   60,   19,   -1,   -1,
  127,   -1,  129,  130,   -1,   -1,   -1,   29,   30,   -1,
   -1,   -1,   -1,   -1,  141,   79,   38,   39,  145,   -1,
  147,   -1,  149,   -1,   -1,   -1,   -1,   91,   92,   93,
   -1,  158,   42,   43,   -1,   45,  163,   47,   -1,   61,
   62,   63,   64,   65,   66,   -1,   -1,   69,   41,   42,
   43,   61,   45,   -1,   47,   41,   42,   43,   -1,   45,
   -1,   47,   41,   42,   43,   -1,   45,   -1,   47,   41,
   42,   43,   -1,   45,  138,   47,   -1,  141,   -1,   -1,
   -1,  103,  104,   -1,   94,   -1,   -1,  109,  110,  153,
  154,   42,   43,   44,   45,   -1,   47,   42,   43,   44,
   45,   94,   47,   -1,   -1,   -1,   -1,   58,   94,  131,
   61,   -1,   -1,   58,   -1,   94,   61,   -1,   42,   43,
   -1,   45,   94,   47,   -1,   42,   43,   -1,   45,   -1,
   47,   42,   43,   -1,   45,   -1,   47,   61,   -1,   -1,
   -1,   -1,   -1,   94,   61,   -1,   -1,   -1,   -1,   94,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   94,   -1,   -1,   -1,   -1,   -1,   -1,   94,   -1,   -1,
   -1,   -1,   -1,   94,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 275
#define YYUNDFTOKEN 296
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,"'#'",0,0,0,0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,
"':'","';'",0,"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'['",0,"']'","'^'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"NAME","NUM","NEG","LOG","EXP","SUM","PROD",
"STRING","BADNUM","SET","VAR","PAR","EQU","LEAD","LAG","NEXT","TIME","FIRST",
"LAST",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : file",
"file : stmt",
"file : file ';' stmt",
"stmt :",
"stmt : declare",
"stmt : eqn",
"declare : decl NAME",
"declare : decl NAME STRING",
"declare : decl NAME attr",
"declare : decl NAME STRING attr",
"declare : decl NAME list STRING",
"declare : decl NAME '(' list ')'",
"declare : decl NAME '(' list ')' STRING",
"declare : decl NAME '(' list ')' attr",
"declare : decl NAME '(' list ')' STRING attr",
"declare : decl NAME '(' list ')' list STRING",
"declare : decset",
"decl : PAR",
"decl : VAR",
"decset : SET tiok '(' list ')'",
"decset : SET tiok '(' list ')' STRING",
"decset : SET NAME '=' NAME",
"decset : SET NAME '=' NAME STRING",
"decset : SET NAME '=' tiok '(' list ')'",
"decset : SET NAME '=' tiok '(' list ')' STRING",
"decset : SET forl '=' TIME '(' item ')'",
"decset : SET forl '=' TIME '(' item ')' STRING",
"decset : SET NAME '=' NAME '+' '(' list ')'",
"decset : SET NAME '=' NAME '+' '(' list ')' STRING",
"decset : SET NAME '=' NAME '-' '(' list ')'",
"decset : SET NAME '=' NAME '-' '(' list ')' STRING",
"tiok : NAME",
"tiok : TIME",
"forl : FIRST",
"forl : LAST",
"eqn : expr '=' expr",
"eqn : expr '=' expr STRING",
"eqn : expr '=' expr attr",
"eqn : expr '=' expr STRING attr",
"eqn : qual expr '=' expr",
"eqn : qual expr '=' expr STRING",
"eqn : qual expr '=' expr attr",
"eqn : qual expr '=' expr STRING attr",
"eqn : eqname expr '=' expr",
"eqn : eqname expr '=' expr STRING",
"eqn : eqname expr '=' expr attr",
"eqn : eqname expr '=' expr STRING attr",
"eqn : eqname qual expr '=' expr",
"eqn : eqname qual expr '=' expr STRING",
"eqn : eqname qual expr '=' expr attr",
"eqn : eqname qual expr '=' expr STRING attr",
"eqname : '/' NAME '/'",
"eqname : EQU NAME",
"qual : list ':'",
"attr : list",
"attr : '{' list '}'",
"attr : '[' list ']'",
"list : item",
"list : item ',' list",
"item : NAME",
"item : NUM",
"item : FIRST",
"item : LAST",
"item : TIME",
"expr : expr '+' expr",
"expr : expr '-' expr",
"expr : expr '*' expr",
"expr : expr '/' expr",
"expr : expr '^' expr",
"expr : '-' expr",
"expr : '(' expr ')'",
"expr : func",
"expr : setfunc",
"expr : lag",
"expr : lead",
"expr : var",
"expr : NUM",
"var : NAME",
"var : NAME '(' list ')'",
"var : NAME '#' NAME",
"var : NAME '(' list ')' '#' NAME",
"func : LOG '(' expr ')'",
"func : EXP '(' expr ')'",
"lag : LAG '(' var ')'",
"lag : LAG '(' lag ')'",
"lead : LEAD '(' var ')'",
"lead : LEAD '(' lead ')'",
"lead : NEXT '(' var ')'",
"lead : NEXT '(' lead ')'",
"setfunc : SUM '(' NAME ',' expr ')'",
"setfunc : PROD '(' NAME ',' expr ')'",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 256 "parse.y"



// error handling is done in readfile rather than here

void yyerror(char *str)
{
}

// recover the last token for error messages

char *getlasttoken()
{
   if( yylval==0 )return 0;
   return snprint(yylval);
}

/*  Include lexical analyzer so yylex and yyparse can share tokens  */

#include "lexical.c"
#line 489 "parse.c"

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yym = 0;
    yyn = 0;
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        yychar = YYLEX;
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;

    YYERROR_CALL("syntax error");

    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);

    switch (yyn)
    {
case 6:
#line 83 "parse.y"
	{ declare(yystack.l_mark[-1],yystack.l_mark[0], 0, 0, 0); }
break;
case 7:
#line 84 "parse.y"
	{ declare(yystack.l_mark[-2],yystack.l_mark[-1], 0,yystack.l_mark[0], 0); }
break;
case 8:
#line 85 "parse.y"
	{ declare(yystack.l_mark[-2],yystack.l_mark[-1], 0, 0,yystack.l_mark[0]); }
break;
case 9:
#line 86 "parse.y"
	{ declare(yystack.l_mark[-3],yystack.l_mark[-2], 0,yystack.l_mark[-1],yystack.l_mark[0]); }
break;
case 10:
#line 87 "parse.y"
	{ declare(yystack.l_mark[-3],yystack.l_mark[-2], 0,yystack.l_mark[0],yystack.l_mark[-1]); }
break;
case 11:
#line 88 "parse.y"
	{ declare(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0, 0); }
break;
case 12:
#line 89 "parse.y"
	{ declare(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0); }
break;
case 13:
#line 90 "parse.y"
	{ declare(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2], 0,yystack.l_mark[0]); }
break;
case 14:
#line 91 "parse.y"
	{ declare(yystack.l_mark[-6],yystack.l_mark[-5],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0]); }
break;
case 15:
#line 92 "parse.y"
	{ declare(yystack.l_mark[-6],yystack.l_mark[-5],yystack.l_mark[-3],yystack.l_mark[0],yystack.l_mark[-1]); }
break;
case 19:
#line 101 "parse.y"
	{ declare(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0, 0) ; }
break;
case 20:
#line 102 "parse.y"
	{ declare(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0) ; }
break;
case 21:
#line 103 "parse.y"
	{ decset (yystack.l_mark[-2],yystack.l_mark[0],nul, 0, 0); }
break;
case 22:
#line 104 "parse.y"
	{ decset (yystack.l_mark[-3],yystack.l_mark[-1],nul, 0,yystack.l_mark[0]); }
break;
case 23:
#line 105 "parse.y"
	{ decset (yystack.l_mark[-5],yystack.l_mark[-3],equ,yystack.l_mark[-1], 0); }
break;
case 24:
#line 106 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],equ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 25:
#line 107 "parse.y"
	{ decset (yystack.l_mark[-5],yystack.l_mark[-3],equ,yystack.l_mark[-1], 0); }
break;
case 26:
#line 108 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],equ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 27:
#line 109 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],add,yystack.l_mark[-1], 0); }
break;
case 28:
#line 110 "parse.y"
	{ decset (yystack.l_mark[-7],yystack.l_mark[-5],add,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 29:
#line 111 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],sub,yystack.l_mark[-1], 0); }
break;
case 30:
#line 112 "parse.y"
	{ decset (yystack.l_mark[-7],yystack.l_mark[-5],sub,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 35:
#line 130 "parse.y"
	{ neweqn( 0,yystack.l_mark[-2],yystack.l_mark[0], 0, 0, 0); }
break;
case 36:
#line 131 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1], 0, 0,yystack.l_mark[0]); }
break;
case 37:
#line 132 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0], 0, 0); }
break;
case 38:
#line 133 "parse.y"
	{ neweqn( 0,yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-1]); }
break;
case 39:
#line 134 "parse.y"
	{ neweqn(yystack.l_mark[-3],yystack.l_mark[-2],yystack.l_mark[0], 0, 0, 0); }
break;
case 40:
#line 135 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0, 0,yystack.l_mark[0]); }
break;
case 41:
#line 136 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0], 0, 0); }
break;
case 42:
#line 137 "parse.y"
	{ neweqn(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-1]); }
break;
case 43:
#line 138 "parse.y"
	{ neweqn( 0,yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-3], 0); }
break;
case 44:
#line 139 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1], 0,yystack.l_mark[-4],yystack.l_mark[0]); }
break;
case 45:
#line 140 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0],yystack.l_mark[-4], 0); }
break;
case 46:
#line 141 "parse.y"
	{ neweqn( 0,yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0],yystack.l_mark[-5],yystack.l_mark[-1]); }
break;
case 47:
#line 142 "parse.y"
	{ neweqn(yystack.l_mark[-3],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-4], 0); }
break;
case 48:
#line 143 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0,yystack.l_mark[-5],yystack.l_mark[0]); }
break;
case 49:
#line 144 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0],yystack.l_mark[-5], 0); }
break;
case 50:
#line 145 "parse.y"
	{ neweqn(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0],yystack.l_mark[-6],yystack.l_mark[-1]); }
break;
case 51:
#line 148 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 52:
#line 149 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 53:
#line 152 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 54:
#line 155 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 55:
#line 156 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 56:
#line 157 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 57:
#line 160 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 58:
#line 161 "parse.y"
	{ yyval = yystack.l_mark[-2]; yystack.l_mark[-2]->r=yystack.l_mark[0]; }
break;
case 64:
#line 189 "parse.y"
	{ yyval = newnode(add, "+"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 65:
#line 190 "parse.y"
	{ yyval = newnode(sub, "-"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 66:
#line 191 "parse.y"
	{ yyval = newnode(mul, "*"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 67:
#line 192 "parse.y"
	{ yyval = newnode(dvd, "/"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 68:
#line 193 "parse.y"
	{ yyval = newnode(pow, "^"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 69:
#line 194 "parse.y"
	{ yyval = newnode(neg, "-"  , 0,yystack.l_mark[0]); }
break;
case 70:
#line 195 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 71:
#line 196 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 72:
#line 197 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 73:
#line 198 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 74:
#line 199 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 75:
#line 200 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 76:
#line 201 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 77:
#line 214 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 78:
#line 215 "parse.y"
	{ yyval = yystack.l_mark[-3]; yystack.l_mark[-3]->r=newnode(lst,"lst",0,yystack.l_mark[-1]); }
break;
case 79:
#line 216 "parse.y"
	{ yyval = newnode(dom,"#",yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 80:
#line 217 "parse.y"
	{ yyval = newnode(dom,"#",yystack.l_mark[-5],yystack.l_mark[0]); yystack.l_mark[-5]->r=newnode(lst,"lst",0,yystack.l_mark[-3]); }
break;
case 81:
#line 231 "parse.y"
	{ yyval = newnode(log,"log", 0,yystack.l_mark[-1]); }
break;
case 82:
#line 232 "parse.y"
	{ yyval = newnode(exp,"exp", 0,yystack.l_mark[-1]); }
break;
case 83:
#line 235 "parse.y"
	{ yyval = newnode(lag,"lag", 0,yystack.l_mark[-1]); }
break;
case 84:
#line 236 "parse.y"
	{ yyval = newnode(lag,"lag", 0,yystack.l_mark[-1]); }
break;
case 85:
#line 239 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 86:
#line 240 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 87:
#line 241 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 88:
#line 242 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 89:
#line 245 "parse.y"
	{ yyval = newnode(sum,"sum"  ,yystack.l_mark[-3],yystack.l_mark[-1]); }
break;
case 90:
#line 246 "parse.y"
	{ yyval = newnode(prd,"prod" ,yystack.l_mark[-3],yystack.l_mark[-1]); }
break;
#line 980 "parse.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            yychar = YYLEX;
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    YYERROR_CALL("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
