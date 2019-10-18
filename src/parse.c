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
#define UNION 276
#define YYERRCODE 256
typedef short YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    0,    1,    1,    1,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    4,    4,    7,    7,
    7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
    7,    7,    7,    7,    7,    7,    8,    8,    9,    9,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,   13,   13,   12,    5,
    5,    5,    6,    6,   10,   10,   10,   10,   10,   11,
   11,   11,   11,   11,   11,   11,   11,   11,   11,   11,
   11,   11,   18,   18,   18,   18,   14,   14,   16,   16,
   17,   17,   17,   17,   15,   15,
};
static const YYINT yylen[] = {                            2,
    1,    3,    0,    1,    1,    2,    3,    3,    4,    4,
    5,    6,    6,    7,    7,    1,    1,    1,    5,    6,
    4,    5,    7,    8,    7,    8,    8,    9,    8,    9,
    6,    7,    6,    7,    7,    8,    1,    1,    1,    1,
    3,    4,    4,    5,    4,    5,    5,    6,    4,    5,
    5,    6,    5,    6,    6,    7,    3,    2,    2,    1,
    3,    3,    1,    3,    1,    1,    1,    1,    1,    3,
    3,    3,    3,    3,    2,    3,    1,    1,    1,    1,
    1,    1,    1,    4,    3,    6,    4,    4,    4,    4,
    4,    4,    4,    4,    6,    6,
};
static const YYINT yydefred[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,   18,   17,    0,
    0,    0,    0,   69,   67,   68,    0,    0,    0,    0,
    1,    4,    5,    0,    0,   16,    0,    0,    0,    0,
   77,   78,   79,   80,   81,    0,    0,    0,    0,    0,
    0,    0,   38,   39,   40,    0,    0,   58,    0,    0,
    0,    0,   82,   75,    0,    0,    0,    0,   59,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   65,
   66,    0,   85,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   57,   76,    2,    0,
    0,    0,    0,    8,    0,   64,    0,    0,    0,    0,
    0,   74,    0,    0,    0,    0,   87,   88,    0,    0,
    0,    0,    0,    0,    0,   92,   91,   90,   89,   94,
   93,    9,   60,    0,    0,    0,   10,    0,   43,    0,
    0,    0,    0,    0,    0,   22,    0,    0,    0,    0,
    0,    0,    0,   61,   62,   44,    0,   47,    0,   51,
    0,   86,   95,   96,    0,    0,    0,    0,    0,    0,
   20,    0,    0,   13,    0,   48,   52,    0,   55,   32,
    0,   34,    0,    0,    0,    0,   14,   15,   56,    0,
    0,   36,   24,   26,   28,   30,
};
static const YYINT yydgoto[] = {                         20,
   21,   22,   23,   24,   94,  123,   26,   46,   47,   27,
   28,   29,   30,   31,   32,   33,   34,   35,
};
static const YYINT yysindex[] = {                       -40,
  -27,    0,  -38,  -30,  -29,   -8, -102,    0,    0, -220,
   -2,    7,   13,    0,    0,    0,  130, -216,  130,   42,
    0,    0,    0, -200,   70,    0,   71,  487,  130,  -21,
    0,    0,    0,    0,    0,  -11, -134,  130,  130, -128,
 -103,   98,    0,    0,    0,  121,  101,    0, -254, -226,
 -254,  -27,    0,    0,  129,  167,  -40,   57,    0,  -11,
  130,  130,  130,  130,  130,  130,  493,  499,  130,    0,
    0,  125,    0,  331,  388,  133,  141, -240,  -11,  -92,
  148,  150,  153,  156,  157,  159,    0,    0,    0,   92,
  -11,  -11,  -11,    0,  -63,    0,   69,   63,   63,  108,
  108,    0,  130,  130,  508,  168,    0,    0,  130,  130,
  -39,  164,  184,  166,  204,    0,    0,    0,    0,    0,
    0,    0,    0,  207,  131,  162,    0,   92,    0,   88,
   96,  130,    3,  424,  432,    0,  -19,  -14,  -11,  -11,
   -7,  -11,   65,    0,    0,    0,   92,    0,   92,    0,
  122,    0,    0,    0,    4,  -11,    5,  -11,  229,  230,
    0,  231,   92,    0,   14,    0,    0,   92,    0,    0,
  236,    0,  240,   21,   26,   36,    0,    0,    0,   37,
   38,    0,    0,    0,    0,    0,
};
static const YYINT yyrindex[] = {                        20,
  464,  470,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   49,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  249,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    1,    0,    0,    0,    0,   20,   23,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   25,
    0,    0,    0,    0,   27,    0,   28,   35,   61,   22,
   30,    0,    0,    0,    0,    9,    0,    0,    0,    0,
   15,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   29,    0,   34,
   39,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   40,    0,   58,    0,    0,    0,   59,    0,   66,    0,
   85,    0,    0,    0,  109,    0,  119,    0,    0,    0,
    0,    0,  127,    0,   27,    0,    0,  134,    0,    0,
    0,    0,    0,  136,  137,  140,    0,    0,    0,  146,
  147,    0,    0,    0,    0,    0,
};
static const YYINT yygindex[] = {                         0,
  234,    0,    0,    0,  313,  347,    0,  220,    0,  165,
  389,  276,    0,    0,    0,  261,  -37,  100,
};
#define YYTABLESIZE 602
static const YYINT yytable[] = {                         19,
   83,   38,   52,  137,   17,  138,   18,   37,   84,   39,
   40,   81,   36,   85,   21,   11,  111,   13,   19,    3,
  156,   72,    6,   17,    7,  158,   60,   41,   42,   73,
   52,   41,   43,   45,   70,  112,   48,   49,   49,   19,
   55,   83,   83,   83,   12,   83,   50,   83,   63,   84,
   84,   84,   51,   84,   37,   84,   58,   11,   46,   83,
   71,   83,   72,   72,   72,   50,   72,   84,   72,   84,
   73,   73,   73,   21,   73,   70,   73,   70,    3,   70,
   72,    6,   72,    7,   53,   60,   41,   42,   73,   63,
   73,   83,   45,   70,   83,   70,   91,   49,   19,   84,
   57,   71,   84,   71,   64,   71,   63,   63,   31,   65,
   64,   62,   72,   63,   60,   65,   11,   46,   33,   71,
   73,   71,   73,   83,   50,   70,   12,   59,   76,   64,
   62,   84,   63,   54,   65,   35,   23,   64,   62,   25,
   63,   63,   65,   53,   72,   27,   29,   93,   82,   84,
   86,   71,   73,   77,   42,   93,   66,   70,   78,   93,
   79,   80,   66,   64,   62,  106,   63,   31,   65,   19,
   43,   44,   45,   63,   17,   87,  109,   33,   93,   92,
  115,   66,   93,   71,  110,   12,   93,   92,  116,   66,
  117,   92,   54,  118,   35,   23,  119,  120,   25,  121,
  127,   66,  133,  139,   27,   29,  141,   88,   64,   62,
   92,   63,   93,   65,   92,   66,    1,    2,   92,    3,
    4,    5,    6,  140,  136,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,    1,    2,  155,    3,    4,
    5,    6,  157,  142,   92,   70,   71,  143,   11,   12,
   13,   14,   15,   16,  145,  144,  161,   83,   83,  152,
   66,   14,   15,   16,   83,   84,   84,  170,  172,  174,
  175,  176,   84,   83,   83,   83,  180,  178,   72,   72,
  181,   84,   84,   84,  182,   72,   73,   73,   37,  183,
   89,   70,   70,   73,   72,   72,   72,  113,   70,  184,
  185,  186,   73,   73,   73,   69,  162,   70,   70,   70,
   83,    0,   63,   70,   71,    0,    0,   71,   71,    0,
   90,   70,   71,    0,   71,   70,   71,    0,  163,   14,
   15,   16,  128,   71,   71,   71,    0,   14,   15,   16,
    0,   14,   15,   16,   70,   71,   25,    0,   70,   71,
    0,  147,   70,   71,    0,    0,    0,    0,    0,  149,
   14,   15,   16,    0,   14,   15,   16,    0,   14,   15,
   16,  107,   64,   62,    0,   63,   25,   65,   70,   71,
    0,    0,   72,    0,    0,  168,   52,   53,    0,    3,
    4,    5,    6,    0,   14,   15,   16,    0,    0,   11,
   12,   13,  122,   25,   95,   54,   96,   56,    0,  129,
    0,    0,    0,    0,    0,    0,    0,   67,   68,    0,
    0,    0,    0,    0,   66,  114,   74,   75,  108,   64,
   62,    0,   63,    0,   65,    0,    0,  124,  125,  126,
  146,    0,  148,  150,    0,    0,    0,    0,    0,   97,
   98,   99,  100,  101,  102,  164,    0,  105,    0,  166,
    0,  167,    0,  169,  153,   64,   62,    0,   63,    0,
   65,    0,  154,   64,   62,  177,   63,    0,   65,    0,
  179,   66,    0,    0,    0,  159,  160,    0,    0,  165,
    0,  130,  131,    0,    0,    0,    0,  134,  135,    0,
    0,    0,  171,    0,  173,   83,   83,   65,   83,    0,
   83,   82,   82,   66,   82,    0,   82,   66,    0,    0,
  151,   65,    0,    0,   83,   66,    0,   66,   64,   62,
   82,   63,    0,   65,   64,   62,    0,   63,    0,   65,
   64,   62,    0,   63,    0,   65,    0,   61,    0,   64,
   62,    0,   63,  103,   65,    0,    0,   83,    0,  104,
    0,    0,    0,   82,    0,    0,    0,    0,  132,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   66,    0,    0,    0,    0,    0,   66,    0,    0,    0,
    0,    0,   66,    0,    0,    0,    0,    0,    0,    0,
    0,   66,
};
static const YYINT yycheck[] = {                         40,
    0,   40,  257,   43,   45,   45,   47,   35,    0,   40,
   40,   49,   40,   51,    0,  270,  257,  272,   40,    0,
   40,    0,    0,   45,    0,   40,    0,    0,    0,    0,
  257,   40,  273,    0,    0,  276,  257,   40,    0,    0,
  257,   41,   42,   43,  271,   45,   40,   47,    0,   41,
   42,   43,   40,   45,   40,   47,  257,    0,    0,   59,
    0,   61,   41,   42,   43,    0,   45,   59,   47,   61,
   41,   42,   43,   59,   45,   41,   47,   43,   59,   45,
   59,   59,   61,   59,    0,   59,   59,   59,   59,   41,
   61,   91,   59,   59,   94,   61,   40,   59,   59,   91,
   59,   41,   94,   43,   42,   45,   58,   59,    0,   47,
   42,   43,   91,   45,   44,   47,   59,   59,    0,   59,
   91,   61,  257,  123,   59,   91,    0,   58,  257,   42,
   43,  123,   45,    0,   47,    0,    0,   42,   43,    0,
   45,   93,   47,   59,  123,    0,    0,   91,   49,   50,
   51,   91,  123,  257,  257,   91,   94,  123,   61,   91,
   40,   61,   94,   42,   43,   41,   45,   59,   47,   40,
  273,  274,  275,  125,   45,   47,   44,   59,   91,  123,
  273,   94,   91,  123,   44,   59,   91,  123,   41,   94,
   41,  123,   59,   41,   59,   59,   41,   41,   59,   41,
  264,   94,   35,   40,   59,   59,   41,   41,   42,   43,
  123,   45,   91,   47,  123,   94,  257,  258,  123,  260,
  261,  262,  263,   40,  264,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  257,  258,  257,  260,  261,
  262,  263,  257,   40,  123,  257,  258,   41,  270,  271,
  272,  273,  274,  275,   93,  125,  264,  257,  258,  257,
   94,  273,  274,  275,  264,  257,  258,  264,  264,   41,
   41,   41,  264,  273,  274,  275,   41,  264,  257,  258,
   41,  273,  274,  275,  264,  264,  257,  258,   40,  264,
   57,  257,  258,  264,  273,  274,  275,   78,  264,  264,
  264,  264,  273,  274,  275,   30,  142,  273,  274,  275,
   50,   -1,  264,  257,  258,   -1,   -1,  257,  258,   -1,
  264,  257,  258,   -1,  264,  257,  258,   -1,  264,  273,
  274,  275,  264,  273,  274,  275,   -1,  273,  274,  275,
   -1,  273,  274,  275,  257,  258,    0,   -1,  257,  258,
   -1,  264,  257,  258,   -1,   -1,   -1,   -1,   -1,  264,
  273,  274,  275,   -1,  273,  274,  275,   -1,  273,  274,
  275,   41,   42,   43,   -1,   45,   30,   47,  257,  258,
   -1,   -1,   36,   -1,   -1,  264,  257,  258,   -1,  260,
  261,  262,  263,   -1,  273,  274,  275,   -1,   -1,  270,
  271,  272,   90,   57,   58,   17,   60,   19,   -1,   97,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   29,   30,   -1,
   -1,   -1,   -1,   -1,   94,   79,   38,   39,   41,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,   91,   92,   93,
  128,   -1,  130,  131,   -1,   -1,   -1,   -1,   -1,   61,
   62,   63,   64,   65,   66,  143,   -1,   69,   -1,  147,
   -1,  149,   -1,  151,   41,   42,   43,   -1,   45,   -1,
   47,   -1,   41,   42,   43,  163,   45,   -1,   47,   -1,
  168,   94,   -1,   -1,   -1,  139,  140,   -1,   -1,  143,
   -1,  103,  104,   -1,   -1,   -1,   -1,  109,  110,   -1,
   -1,   -1,  156,   -1,  158,   42,   43,   44,   45,   -1,
   47,   42,   43,   44,   45,   -1,   47,   94,   -1,   -1,
  132,   58,   -1,   -1,   61,   94,   -1,   58,   42,   43,
   61,   45,   -1,   47,   42,   43,   -1,   45,   -1,   47,
   42,   43,   -1,   45,   -1,   47,   -1,   61,   -1,   42,
   43,   -1,   45,   61,   47,   -1,   -1,   94,   -1,   61,
   -1,   -1,   -1,   94,   -1,   -1,   -1,   -1,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   94,   -1,   -1,   -1,   -1,   -1,   94,   -1,   -1,   -1,
   -1,   -1,   94,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   94,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 276
#define YYUNDFTOKEN 297
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
"LAST","UNION",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"illegal-symbol",
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
"decset : SET NAME '=' NAME '+' NAME",
"decset : SET NAME '=' NAME '+' NAME STRING",
"decset : SET NAME '=' NAME '-' NAME",
"decset : SET NAME '=' NAME '-' NAME STRING",
"decset : SET NAME '=' UNION '(' list ')'",
"decset : SET NAME '=' UNION '(' list ')' STRING",
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
#line 266 "parse.y"



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
#line 503 "parse.c"

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
#line 87 "parse.y"
	{ declare(yystack.l_mark[-1],yystack.l_mark[0], 0, 0, 0); }
break;
case 7:
#line 88 "parse.y"
	{ declare(yystack.l_mark[-2],yystack.l_mark[-1], 0,yystack.l_mark[0], 0); }
break;
case 8:
#line 89 "parse.y"
	{ declare(yystack.l_mark[-2],yystack.l_mark[-1], 0, 0,yystack.l_mark[0]); }
break;
case 9:
#line 90 "parse.y"
	{ declare(yystack.l_mark[-3],yystack.l_mark[-2], 0,yystack.l_mark[-1],yystack.l_mark[0]); }
break;
case 10:
#line 91 "parse.y"
	{ declare(yystack.l_mark[-3],yystack.l_mark[-2], 0,yystack.l_mark[0],yystack.l_mark[-1]); }
break;
case 11:
#line 92 "parse.y"
	{ declare(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0, 0); }
break;
case 12:
#line 93 "parse.y"
	{ declare(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0); }
break;
case 13:
#line 94 "parse.y"
	{ declare(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2], 0,yystack.l_mark[0]); }
break;
case 14:
#line 95 "parse.y"
	{ declare(yystack.l_mark[-6],yystack.l_mark[-5],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0]); }
break;
case 15:
#line 96 "parse.y"
	{ declare(yystack.l_mark[-6],yystack.l_mark[-5],yystack.l_mark[-3],yystack.l_mark[0],yystack.l_mark[-1]); }
break;
case 19:
#line 105 "parse.y"
	{ declare(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0, 0) ; }
break;
case 20:
#line 106 "parse.y"
	{ declare(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0) ; }
break;
case 21:
#line 107 "parse.y"
	{ decset (yystack.l_mark[-2],yystack.l_mark[0],nul, 0, 0); }
break;
case 22:
#line 108 "parse.y"
	{ decset (yystack.l_mark[-3],yystack.l_mark[-1],nul, 0,yystack.l_mark[0]); }
break;
case 23:
#line 109 "parse.y"
	{ decset (yystack.l_mark[-5],yystack.l_mark[-3],equ,yystack.l_mark[-1], 0); }
break;
case 24:
#line 110 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],equ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 25:
#line 111 "parse.y"
	{ decset (yystack.l_mark[-5],yystack.l_mark[-3],equ,yystack.l_mark[-1], 0); }
break;
case 26:
#line 112 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],equ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 27:
#line 113 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],add,yystack.l_mark[-1], 0); }
break;
case 28:
#line 114 "parse.y"
	{ decset (yystack.l_mark[-7],yystack.l_mark[-5],add,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 29:
#line 115 "parse.y"
	{ decset (yystack.l_mark[-6],yystack.l_mark[-4],sub,yystack.l_mark[-1], 0); }
break;
case 30:
#line 116 "parse.y"
	{ decset (yystack.l_mark[-7],yystack.l_mark[-5],sub,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 31:
#line 117 "parse.y"
	{ decset (yystack.l_mark[-4],yystack.l_mark[-2],sad,yystack.l_mark[0], 0); }
break;
case 32:
#line 118 "parse.y"
	{ decset (yystack.l_mark[-5],yystack.l_mark[-3],sad,yystack.l_mark[-1],yystack.l_mark[0]); }
break;
case 33:
#line 119 "parse.y"
	{ decset (yystack.l_mark[-4],yystack.l_mark[-2],ssu,yystack.l_mark[0], 0); }
break;
case 34:
#line 120 "parse.y"
	{ decset (yystack.l_mark[-5],yystack.l_mark[-3],ssu,yystack.l_mark[-1],yystack.l_mark[0]); }
break;
case 35:
#line 121 "parse.y"
	{ decunion(yystack.l_mark[-5],yystack.l_mark[-1], 0); }
break;
case 36:
#line 122 "parse.y"
	{ decunion(yystack.l_mark[-6],yystack.l_mark[-2],yystack.l_mark[-1]); }
break;
case 41:
#line 140 "parse.y"
	{ neweqn( 0,yystack.l_mark[-2],yystack.l_mark[0], 0, 0, 0); }
break;
case 42:
#line 141 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1], 0, 0,yystack.l_mark[0]); }
break;
case 43:
#line 142 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0], 0, 0); }
break;
case 44:
#line 143 "parse.y"
	{ neweqn( 0,yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-1]); }
break;
case 45:
#line 144 "parse.y"
	{ neweqn(yystack.l_mark[-3],yystack.l_mark[-2],yystack.l_mark[0], 0, 0, 0); }
break;
case 46:
#line 145 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0, 0,yystack.l_mark[0]); }
break;
case 47:
#line 146 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0], 0, 0); }
break;
case 48:
#line 147 "parse.y"
	{ neweqn(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-1]); }
break;
case 49:
#line 148 "parse.y"
	{ neweqn( 0,yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-3], 0); }
break;
case 50:
#line 149 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1], 0,yystack.l_mark[-4],yystack.l_mark[0]); }
break;
case 51:
#line 150 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0],yystack.l_mark[-4], 0); }
break;
case 52:
#line 151 "parse.y"
	{ neweqn( 0,yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0],yystack.l_mark[-5],yystack.l_mark[-1]); }
break;
case 53:
#line 152 "parse.y"
	{ neweqn(yystack.l_mark[-3],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-4], 0); }
break;
case 54:
#line 153 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0,yystack.l_mark[-5],yystack.l_mark[0]); }
break;
case 55:
#line 154 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0],yystack.l_mark[-5], 0); }
break;
case 56:
#line 155 "parse.y"
	{ neweqn(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0],yystack.l_mark[-6],yystack.l_mark[-1]); }
break;
case 57:
#line 158 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 58:
#line 159 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 59:
#line 162 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 60:
#line 165 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 61:
#line 166 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 62:
#line 167 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 63:
#line 170 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 64:
#line 171 "parse.y"
	{ yyval = yystack.l_mark[-2]; yystack.l_mark[-2]->r=yystack.l_mark[0]; }
break;
case 70:
#line 199 "parse.y"
	{ yyval = newnode(add, "+"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 71:
#line 200 "parse.y"
	{ yyval = newnode(sub, "-"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 72:
#line 201 "parse.y"
	{ yyval = newnode(mul, "*"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 73:
#line 202 "parse.y"
	{ yyval = newnode(dvd, "/"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 74:
#line 203 "parse.y"
	{ yyval = newnode(pow, "^"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 75:
#line 204 "parse.y"
	{ yyval = newnode(neg, "-"  , 0,yystack.l_mark[0]); }
break;
case 76:
#line 205 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 77:
#line 206 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 78:
#line 207 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 79:
#line 208 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 80:
#line 209 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 81:
#line 210 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 82:
#line 211 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 83:
#line 224 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 84:
#line 225 "parse.y"
	{ yyval = yystack.l_mark[-3]; yystack.l_mark[-3]->r=newnode(lst,"lst",0,yystack.l_mark[-1]); }
break;
case 85:
#line 226 "parse.y"
	{ yyval = newnode(dom,"#",yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 86:
#line 227 "parse.y"
	{ yyval = newnode(dom,"#",yystack.l_mark[-5],yystack.l_mark[0]); yystack.l_mark[-5]->r=newnode(lst,"lst",0,yystack.l_mark[-3]); }
break;
case 87:
#line 241 "parse.y"
	{ yyval = newnode(log,"log", 0,yystack.l_mark[-1]); }
break;
case 88:
#line 242 "parse.y"
	{ yyval = newnode(exp,"exp", 0,yystack.l_mark[-1]); }
break;
case 89:
#line 245 "parse.y"
	{ yyval = newnode(lag,"lag", 0,yystack.l_mark[-1]); }
break;
case 90:
#line 246 "parse.y"
	{ yyval = newnode(lag,"lag", 0,yystack.l_mark[-1]); }
break;
case 91:
#line 249 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 92:
#line 250 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 93:
#line 251 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 94:
#line 252 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 95:
#line 255 "parse.y"
	{ yyval = newnode(sum,"sum"  ,yystack.l_mark[-3],yystack.l_mark[-1]); }
break;
case 96:
#line 256 "parse.y"
	{ yyval = newnode(prd,"prod" ,yystack.l_mark[-3],yystack.l_mark[-1]); }
break;
#line 1018 "parse.c"
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
