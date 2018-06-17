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
    3,    3,   13,   13,   12,    5,    5,    5,    6,    6,
   10,   10,   10,   10,   10,   11,   11,   11,   11,   11,
   11,   11,   11,   11,   11,   11,   11,   11,   18,   18,
   18,   18,   14,   14,   16,   16,   17,   17,   17,   17,
   15,   15,
};
static const YYINT yylen[] = {                            2,
    1,    3,    0,    1,    1,    2,    3,    3,    4,    4,
    5,    6,    6,    7,    7,    1,    1,    1,    5,    6,
    4,    5,    7,    8,    7,    8,    8,    9,    8,    9,
    1,    1,    1,    1,    3,    4,    4,    5,    4,    5,
    5,    6,    3,    2,    2,    1,    3,    3,    1,    3,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    2,    3,    1,    1,    1,    1,    1,    1,    1,    4,
    3,    6,    4,    4,    4,    4,    4,    4,    4,    4,
    6,    6,
};
static const YYINT yydefred[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,   18,   17,    0,
    0,    0,    0,   55,   53,   54,    0,    0,    0,    0,
    1,    4,    5,    0,    0,   16,    0,    0,    0,    0,
   63,   64,   65,   66,   67,    0,    0,    0,    0,    0,
    0,    0,   32,   33,   34,    0,    0,   44,    0,    0,
    0,    0,   68,   61,    0,    0,    0,    0,   45,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   51,
   52,    0,   71,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   43,   62,    2,    0,
    0,    0,    0,    8,    0,   50,    0,    0,    0,    0,
    0,   60,    0,    0,    0,    0,   73,   74,    0,    0,
    0,    0,    0,    0,   78,   77,   76,   75,   80,   79,
    9,   46,    0,    0,    0,   10,   36,    0,    0,    0,
    0,    0,    0,   22,    0,    0,    0,    0,    0,    0,
   47,   48,   38,   40,    0,   72,   81,   82,    0,    0,
    0,   20,    0,    0,   13,    0,   42,    0,    0,    0,
    0,   14,   15,    0,    0,   24,   26,   28,   30,
};
static const YYINT yydgoto[] = {                         20,
   21,   22,   23,   24,   94,  122,   26,   46,   47,   27,
   28,   29,   30,   31,   32,   33,   34,   35,
};
static const YYINT yysindex[] = {                       -40,
   -4,    0,  -30,  -13,  -10,   -1, -127,    0,    0, -246,
   12,   14,   16,    0,    0,    0,   89, -217,   89,    2,
    0,    0,    0, -199,    6,    0,   61,  287,   89,  -21,
    0,    0,    0,    0,    0,  159, -147,   89,   89, -131,
 -107,   90,    0,    0,    0,  113,   93,    0, -237, -254,
 -237,   -4,    0,    0,  112,  -29,  -40,   -2,    0,  159,
   89,   89,   89,   89,   89,   89,  342,  365,   89,    0,
    0,  121,    0,   98,  163,  122,  127, -248,  159, -110,
  133,  138,  139,  142,  146,  147,    0,    0,    0,  -76,
  159,  159,  159,    0,  -88,    0,   70,  130,  130,   96,
   96,    0,   89,   89,  406,  156,    0,    0,   89,   89,
  -39,  167,  162,  169,    0,    0,    0,    0,    0,    0,
    0,    0,  170,   87,  120,    0,    0,   70,   70,   89,
  -43,  274,  335,    0,  175,  176,  159,  -45,  159,  -89,
    0,    0,    0,    0,   70,    0,    0,    0,  159,  159,
  197,    0,  202,  -76,    0,  -20,    0,  204,  205,  -17,
  -16,    0,    0,   -3,    3,    0,    0,    0,    0,
};
static const YYINT yyrindex[] = {                        22,
  348,  377,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   45,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  220,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    1,    0,    0,    0,    0,   22,   26,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   41,
    0,    0,    0,    0,   57,    0,   59,   32,   66,   29,
   37,    0,    0,    0,    0,    8,    0,    0,    0,    0,
   28,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   63,   83,    0,
    0,    0,    0,    0,    0,    0,    0,   97,    0,  106,
    0,    0,    0,    0,  108,    0,    0,    0,    0,    0,
    0,    0,    0,  114,    0,   57,    0,    0,    0,  119,
  135,    0,    0,  136,  137,    0,    0,    0,    0,
};
static const YYINT yygindex[] = {                         0,
  206,    0,    0,    0,    4,  278,    0,  186,    0,  129,
  336,  239,    0,    0,    0,  227,  -28,  151,
};
#define YYTABLESIZE 500
static const YYINT yytable[] = {                         19,
   69,   93,   52,  135,   17,  136,   18,   70,  111,   38,
   48,   88,   64,   62,   93,   63,   12,   65,   19,   52,
   81,    3,   85,   17,   43,    6,   39,   21,   58,   40,
   37,   56,   11,   92,   13,   36,   59,   91,   41,   55,
    7,   69,   69,   69,   49,   69,   92,   69,   70,   70,
   70,   49,   70,   50,   70,   51,   46,   58,   35,   69,
   57,   69,   37,   59,   66,   57,   70,   31,   70,   58,
   58,   58,   56,   58,   56,   58,   56,   59,   59,   59,
    3,   59,   39,   59,    6,   49,   21,   58,   93,   58,
   56,   69,   56,  121,   69,   59,   19,   59,   70,    7,
  127,   70,   49,   49,   60,   11,   57,   41,   57,   73,
   57,   64,   62,   12,   63,   46,   65,   35,   23,   58,
   92,   37,   56,   69,   57,   76,   57,   59,   19,   42,
   70,  143,  144,   17,   25,   27,   29,   49,  107,   64,
   62,   39,   63,  155,   65,   43,   44,   45,  157,   77,
   78,   58,   79,   80,   56,   19,   57,  162,   87,   59,
   93,  106,  114,   66,   11,  109,   41,   70,   71,   49,
  110,   64,   12,  115,  154,  126,   65,   23,  116,  117,
   70,   71,  118,   14,   15,   16,  119,  120,   57,   66,
  131,   66,   92,   25,   27,   29,   14,   15,   16,   82,
   84,   86,  138,  108,   64,   62,  137,   63,  139,   65,
  140,  141,  142,  146,  149,  150,    1,    2,  152,    3,
    4,    5,    6,   66,  134,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,    1,    2,  160,    3,    4,
    5,    6,  161,  163,  164,  165,  166,  167,   11,   12,
   13,   14,   15,   16,   70,   71,   66,   69,   69,   31,
  168,   90,   89,  112,   70,   70,  169,  153,   69,    0,
   14,   15,   16,   69,   69,   69,   83,   25,    0,    0,
   70,   70,   70,    0,    0,   58,   58,    0,   56,   56,
    0,    0,    0,   59,   59,    0,    0,    0,    0,    0,
    0,   58,   58,   58,   56,   56,   56,   25,   49,   59,
   59,   59,    0,   72,  147,   64,   62,    0,   63,    0,
   65,    0,   57,   57,    0,    0,   70,   71,   64,   62,
    0,   63,    0,   65,   25,   95,    0,   96,   57,   57,
   57,    0,   14,   15,   16,   52,   53,   61,    3,    4,
    5,    6,   54,    0,   56,    0,  113,    0,   11,   12,
   13,    0,    0,    0,   67,   68,    0,   66,  123,  124,
  125,    0,    0,   74,   75,  148,   64,   62,    0,   63,
   66,   65,    0,   64,   62,    0,   63,    0,   65,   69,
   69,   51,   69,    0,   69,    0,   97,   98,   99,  100,
  101,  102,  103,    0,  105,   51,   64,   62,   69,   63,
    0,   65,    0,    0,  151,   70,   71,  156,   68,   68,
   52,   68,    0,   68,    0,  104,  158,  159,   66,    0,
    0,   14,   15,   16,   52,   66,    0,   68,  128,  129,
    0,   69,    0,    0,  132,  133,    0,   64,   62,    0,
   63,    0,   65,    0,    0,    0,    0,    0,   66,    0,
    0,    0,    0,    0,    0,  145,  130,    0,    0,    0,
   68,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   66,
};
static const YYINT yycheck[] = {                         40,
    0,   91,  257,   43,   45,   45,   47,    0,  257,   40,
  257,   41,   42,   43,   91,   45,  271,   47,   40,  257,
   49,    0,   51,   45,  273,    0,   40,    0,    0,   40,
   35,    0,  270,  123,  272,   40,    0,   40,   40,  257,
    0,   41,   42,   43,    0,   45,  123,   47,   41,   42,
   43,   40,   45,   40,   47,   40,    0,  257,    0,   59,
   59,   61,    0,   58,   94,    0,   59,   40,   61,   41,
   42,   43,   41,   45,   43,   47,   45,   41,   42,   43,
   59,   45,    0,   47,   59,   41,   59,   59,   91,   61,
   59,   91,   61,   90,   94,   59,    0,   61,   91,   59,
   97,   94,   58,   59,   44,    0,   41,    0,   43,  257,
   45,   42,   43,    0,   45,   59,   47,   59,    0,   91,
  123,   59,   91,  123,   59,  257,   61,   91,   40,  257,
  123,  128,  129,   45,    0,    0,    0,   93,   41,   42,
   43,   59,   45,  140,   47,  273,  274,  275,  145,  257,
   61,  123,   40,   61,  123,   59,   91,  154,   47,  123,
   91,   41,  273,   94,   59,   44,   59,  257,  258,  125,
   44,   42,   59,   41,  264,  264,   47,   59,   41,   41,
  257,  258,   41,  273,  274,  275,   41,   41,  123,   94,
   35,   94,  123,   59,   59,   59,  273,  274,  275,   49,
   50,   51,   41,   41,   42,   43,   40,   45,   40,   47,
   41,  125,   93,  257,   40,   40,  257,  258,  264,  260,
  261,  262,  263,   94,  264,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  257,  258,   41,  260,  261,
  262,  263,   41,  264,   41,   41,  264,  264,  270,  271,
  272,  273,  274,  275,  257,  258,   94,  257,  258,   40,
  264,  264,   57,   78,  257,  258,  264,  139,   30,   -1,
  273,  274,  275,  273,  274,  275,   50,    0,   -1,   -1,
  273,  274,  275,   -1,   -1,  257,  258,   -1,  257,  258,
   -1,   -1,   -1,  257,  258,   -1,   -1,   -1,   -1,   -1,
   -1,  273,  274,  275,  273,  274,  275,   30,  264,  273,
  274,  275,   -1,   36,   41,   42,   43,   -1,   45,   -1,
   47,   -1,  257,  258,   -1,   -1,  257,  258,   42,   43,
   -1,   45,   -1,   47,   57,   58,   -1,   60,  273,  274,
  275,   -1,  273,  274,  275,  257,  258,   61,  260,  261,
  262,  263,   17,   -1,   19,   -1,   79,   -1,  270,  271,
  272,   -1,   -1,   -1,   29,   30,   -1,   94,   91,   92,
   93,   -1,   -1,   38,   39,   41,   42,   43,   -1,   45,
   94,   47,   -1,   42,   43,   -1,   45,   -1,   47,   42,
   43,   44,   45,   -1,   47,   -1,   61,   62,   63,   64,
   65,   66,   61,   -1,   69,   58,   42,   43,   61,   45,
   -1,   47,   -1,   -1,  137,  257,  258,  140,   42,   43,
   44,   45,   -1,   47,   -1,   61,  149,  150,   94,   -1,
   -1,  273,  274,  275,   58,   94,   -1,   61,  103,  104,
   -1,   94,   -1,   -1,  109,  110,   -1,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   94,   -1,
   -1,   -1,   -1,   -1,   -1,  130,   61,   -1,   -1,   -1,
   94,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   94,
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
"eqn : expr '=' expr attr",
"eqn : qual expr '=' expr",
"eqn : qual expr '=' expr attr",
"eqn : eqname expr '=' expr",
"eqn : eqname expr '=' expr attr",
"eqn : eqname qual expr '=' expr",
"eqn : eqname qual expr '=' expr attr",
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
#line 240 "parse.y"



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
#line 458 "parse.c"

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
#line 82 "parse.y"
	{ declare(yystack.l_mark[-1],yystack.l_mark[0], 0, 0, 0); }
break;
case 7:
#line 83 "parse.y"
	{ declare(yystack.l_mark[-2],yystack.l_mark[-1], 0,yystack.l_mark[0], 0); }
break;
case 8:
#line 84 "parse.y"
	{ declare(yystack.l_mark[-2],yystack.l_mark[-1], 0, 0,yystack.l_mark[0]); }
break;
case 9:
#line 85 "parse.y"
	{ declare(yystack.l_mark[-3],yystack.l_mark[-2], 0,yystack.l_mark[-1],yystack.l_mark[0]); }
break;
case 10:
#line 86 "parse.y"
	{ declare(yystack.l_mark[-3],yystack.l_mark[-2], 0,yystack.l_mark[0],yystack.l_mark[-1]); }
break;
case 11:
#line 87 "parse.y"
	{ declare(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0, 0); }
break;
case 12:
#line 88 "parse.y"
	{ declare(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0); }
break;
case 13:
#line 89 "parse.y"
	{ declare(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2], 0,yystack.l_mark[0]); }
break;
case 14:
#line 90 "parse.y"
	{ declare(yystack.l_mark[-6],yystack.l_mark[-5],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0]); }
break;
case 15:
#line 91 "parse.y"
	{ declare(yystack.l_mark[-6],yystack.l_mark[-5],yystack.l_mark[-3],yystack.l_mark[0],yystack.l_mark[-1]); }
break;
case 19:
#line 100 "parse.y"
	{ declare(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0, 0) ; }
break;
case 20:
#line 101 "parse.y"
	{ declare(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0) ; }
break;
case 21:
#line 102 "parse.y"
	{ decset (yystack.l_mark[-2],yystack.l_mark[0],nul, 0, 0); }
break;
case 22:
#line 103 "parse.y"
	{ decset (yystack.l_mark[-3],yystack.l_mark[-1],nul, 0,yystack.l_mark[0]); }
break;
case 23:
#line 104 "parse.y"
	{ decset (yystack.l_mark[-5],yystack.l_mark[-3],equ,yystack.l_mark[-1], 0); }
break;
case 24:
#line 105 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],equ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 25:
#line 106 "parse.y"
	{ decset (yystack.l_mark[-5],yystack.l_mark[-3],equ,yystack.l_mark[-1], 0); }
break;
case 26:
#line 107 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],equ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 27:
#line 108 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],add,yystack.l_mark[-1], 0); }
break;
case 28:
#line 109 "parse.y"
	{ decset (yystack.l_mark[-7],yystack.l_mark[-5],add,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 29:
#line 110 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],sub,yystack.l_mark[-1], 0); }
break;
case 30:
#line 111 "parse.y"
	{ decset (yystack.l_mark[-7],yystack.l_mark[-5],sub,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 35:
#line 129 "parse.y"
	{ neweqn( 0,yystack.l_mark[-2],yystack.l_mark[0], 0, 0); }
break;
case 36:
#line 130 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0], 0); }
break;
case 37:
#line 131 "parse.y"
	{ neweqn(yystack.l_mark[-3],yystack.l_mark[-2],yystack.l_mark[0], 0, 0); }
break;
case 38:
#line 132 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0], 0); }
break;
case 39:
#line 133 "parse.y"
	{ neweqn( 0,yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-3]); }
break;
case 40:
#line 134 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0],yystack.l_mark[-4]); }
break;
case 41:
#line 135 "parse.y"
	{ neweqn(yystack.l_mark[-3],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-4]); }
break;
case 42:
#line 136 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0],yystack.l_mark[-5]); }
break;
case 43:
#line 139 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 44:
#line 140 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 45:
#line 143 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 46:
#line 146 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 47:
#line 147 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 48:
#line 148 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 49:
#line 151 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 50:
#line 152 "parse.y"
	{ yyval = yystack.l_mark[-2]; yystack.l_mark[-2]->r=yystack.l_mark[0]; }
break;
case 56:
#line 180 "parse.y"
	{ yyval = newnode(add, "+"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 57:
#line 181 "parse.y"
	{ yyval = newnode(sub, "-"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 58:
#line 182 "parse.y"
	{ yyval = newnode(mul, "*"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 59:
#line 183 "parse.y"
	{ yyval = newnode(dvd, "/"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 60:
#line 184 "parse.y"
	{ yyval = newnode(pow, "^"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 61:
#line 185 "parse.y"
	{ yyval = newnode(neg, "-"  , 0,yystack.l_mark[0]); }
break;
case 62:
#line 186 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 63:
#line 187 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 64:
#line 188 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 65:
#line 189 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 66:
#line 190 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 67:
#line 191 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 68:
#line 192 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 69:
#line 205 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 70:
#line 206 "parse.y"
	{ yyval = yystack.l_mark[-3]; yystack.l_mark[-3]->r=newnode(lst,"lst",0,yystack.l_mark[-1]); }
break;
case 71:
#line 207 "parse.y"
	{ yyval = newnode(dom,"#",yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 72:
#line 208 "parse.y"
	{ yyval = newnode(dom,"#",yystack.l_mark[-5],yystack.l_mark[0]); yystack.l_mark[-5]->r=newnode(lst,"lst",0,yystack.l_mark[-3]); }
break;
case 73:
#line 222 "parse.y"
	{ yyval = newnode(log,"log", 0,yystack.l_mark[-1]); }
break;
case 74:
#line 223 "parse.y"
	{ yyval = newnode(exp,"exp", 0,yystack.l_mark[-1]); }
break;
case 75:
#line 226 "parse.y"
	{ yyval = newnode(lag,"lag", 0,yystack.l_mark[-1]); }
break;
case 76:
#line 227 "parse.y"
	{ yyval = newnode(lag,"lag", 0,yystack.l_mark[-1]); }
break;
case 77:
#line 230 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 78:
#line 231 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 79:
#line 232 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 80:
#line 233 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 81:
#line 236 "parse.y"
	{ yyval = newnode(sum,"sum"  ,yystack.l_mark[-3],yystack.l_mark[-1]); }
break;
case 82:
#line 237 "parse.y"
	{ yyval = newnode(prd,"prod" ,yystack.l_mark[-3],yystack.l_mark[-1]); }
break;
#line 917 "parse.c"
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
