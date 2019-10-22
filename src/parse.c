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
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,   13,   13,   12,    5,    5,    5,
    6,    6,   10,   10,   10,   10,   10,   11,   11,   11,
   11,   11,   11,   11,   11,   11,   11,   11,   11,   11,
   18,   18,   18,   18,   14,   14,   16,   16,   17,   17,
   17,   17,   15,   15,
};
static const YYINT yylen[] = {                            2,
    1,    3,    0,    1,    1,    2,    3,    3,    4,    4,
    5,    6,    6,    7,    7,    1,    1,    1,    5,    6,
    4,    5,    7,    8,    7,    8,    8,    9,    8,    9,
    6,    7,    6,    7,    7,    8,    1,    1,    1,    1,
    3,    4,    4,    4,    5,    5,    4,    5,    5,    5,
    6,    6,    4,    5,    5,    5,    6,    6,    5,    6,
    6,    6,    7,    7,    3,    2,    2,    1,    3,    3,
    1,    3,    1,    1,    1,    1,    1,    3,    3,    3,
    3,    3,    2,    3,    1,    1,    1,    1,    1,    1,
    1,    4,    3,    6,    4,    4,    4,    4,    4,    4,
    4,    4,    6,    6,
};
static const YYINT yydefred[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,   18,   17,
    0,    0,    0,    0,   77,   75,   76,    0,    0,    0,
    0,    1,    4,    5,    0,    0,   16,    0,    0,    0,
    0,   85,   86,   87,   88,   89,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   38,   39,   40,    0,
    0,   66,    0,    0,    0,    0,   90,   83,    0,    0,
    0,    0,   67,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   73,   74,    0,   93,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   65,   84,    2,    0,    0,    0,
    0,    8,    0,   72,    0,    0,    0,    0,    0,   82,
    0,    0,    0,    0,   95,   96,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  100,   99,   98,
   97,  102,  101,    9,   68,    0,    0,    0,   10,    0,
   44,    0,    0,    0,    0,    0,    0,   46,    0,    0,
    0,   22,    0,    0,    0,    0,    0,    0,    0,   69,
   70,   45,    0,   50,    0,   56,    0,   94,  103,  104,
   52,   58,    0,    0,    0,    0,    0,    0,    0,   20,
    0,    0,   13,    0,   51,   57,    0,   62,   64,   32,
    0,   34,    0,    0,    0,    0,   14,   15,   63,    0,
    0,   36,   24,   26,   28,   30,
};
static const YYINT yydgoto[] = {                         21,
   22,   23,   24,   25,  102,  135,   27,   50,   51,   28,
   29,   30,   31,   32,   33,   34,   35,   36,
};
static const YYINT yysindex[] = {                       -40,
  -27,    0,  -26,  -20,   -8,   -3,  -21, -126,    0,    0,
 -216,    7,   65,   75,    0,    0,    0,  150, -134,  150,
   80,    0,    0,    0, -107,   93,    0,  117,  384,  150,
   69,    0,    0,    0,    0,    0,  102,  -95,  150,  150,
  -92,  -90,  484,  150,   69,  111,    0,    0,    0,  131,
  115,    0, -254, -226, -254,  -27,    0,    0,  133,  346,
  -40,   88,    0,  102,  150,  150,  150,  150,  150,  150,
  576,  582,  150,    0,    0,  136,    0,  356,  426,  143,
  149,  150,  588,  597,  150, -240,  102,  -76,  160,  161,
  162,  163,  169,  172,    0,    0,    0,  -12,  102,  102,
  102,    0,  -48,    0,   91,  213,  213,  170,  170,    0,
  150,  150,  603,  208,    0,    0,  150,  150,  339,  150,
  150,  612,  -39,  216,  217,  227,  229,    0,    0,    0,
    0,    0,    0,    0,    0,  231,  145,  184,    0,  -12,
    0,  121,  311,  150,   21,  455,  469,    0,  339,  339,
  150,    0,  -38,  -19,  102,  102,   17,  102,  -66,    0,
    0,    0,  -12,    0,  -12,    0,  325,    0,    0,    0,
    0,    0,  339,   25,  102,   26,  102,  250,  257,    0,
  259,  -12,    0,   37,    0,    0,  -12,    0,    0,    0,
  261,    0,  265,   47,   48,   50,    0,    0,    0,   51,
   52,    0,    0,    0,    0,    0,
};
static const YYINT yyrindex[] = {                        23,
  112,  559,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   49,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    1,    0,    0,    0,    0,
   23,   27,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   28,    0,    0,
    0,    0,   29,    0,   34,   35,   61,   22,   30,    0,
    0,    0,    0,    9,    0,    0,    0,    0,   38,    0,
    0,    0,   15,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   39,
    0,   40,   53,    0,    0,    0,    0,    0,   58,   59,
    0,    0,    0,    0,    0,    0,   66,    0,   84,    0,
    0,    0,   85,    0,  101,    0,  110,    0,    0,    0,
    0,    0,  116,  119,    0,  127,    0,    0,    0,    0,
    0,  129,    0,   29,    0,    0,  130,    0,    0,    0,
    0,    0,    0,  135,  137,  140,    0,    0,    0,  141,
  146,    0,    0,    0,    0,    0,
};
static const YYINT yygindex[] = {                         0,
  260,    0,    0,    0,  394,  418,    0,  234,    0,  164,
  421,  240,  316,    0,    0,  270,  128,  -43,
};
#define YYTABLESIZE 706
static const YYINT yytable[] = {                         20,
   91,  175,   56,  153,   18,  154,   19,   38,   92,   90,
   92,   94,   37,   39,   21,   12,  123,   14,   20,   40,
  177,   80,    3,   18,  101,   19,    6,    7,   68,   81,
   56,   41,   47,   41,   78,  124,   42,   43,   42,   47,
   52,   91,   91,   91,   13,   91,   53,   91,   71,   92,
   92,   92,   53,   92,   37,   92,  100,   49,   55,   91,
   79,   91,   80,   80,   80,   19,   80,   92,   80,   92,
   81,   81,   81,   21,   81,   78,   81,   78,  101,   78,
   80,    3,   80,   11,   48,    6,    7,   68,   81,   71,
   81,   91,   41,   78,   91,   78,   43,   42,   47,   92,
   54,   79,   92,   79,   54,   79,   71,   71,   20,   59,
  100,   53,   80,   18,   55,   61,   49,   55,   31,   79,
   81,   79,   59,   91,   19,   78,   33,   99,   12,   60,
   46,   92,   68,   66,   35,   67,   23,   69,   61,   25,
   27,   71,   11,   48,   80,   29,   47,   48,   49,   62,
   63,   79,   81,   91,   91,   73,   91,   78,   91,   54,
   64,   77,   68,   66,   80,   67,   81,   69,   59,   73,
   87,   86,   91,   71,   61,   88,  114,   31,  101,   95,
   89,  101,   93,   79,   70,   33,  117,   12,   60,   20,
   74,   75,  118,   35,   18,   23,  127,  182,   25,   27,
  128,  129,  130,  131,   29,   91,   15,   16,   17,  132,
  100,  101,  133,  100,   70,  139,    1,    2,  174,    3,
    4,    5,    6,    7,  152,    8,    9,   10,   11,   12,
   13,   14,   15,   16,   17,    1,    2,  176,    3,    4,
    5,    6,  145,  100,   74,   75,   44,   11,   12,   13,
   14,   15,   16,   17,   68,  155,  156,   91,   91,   69,
   15,   16,   17,   70,   91,   92,   92,  157,  158,  160,
   73,  159,   92,   91,   91,   91,  161,  168,   80,   80,
  180,   92,   92,   92,   85,   80,   81,   81,  190,  192,
  194,   78,   78,   81,   80,   80,   80,  195,   78,  196,
  198,  200,   81,   81,   81,  201,   70,   78,   78,   78,
  202,  203,   71,  204,  205,  206,   37,   79,   79,  125,
   97,  181,   45,   91,   79,    1,    2,    0,    3,    4,
    5,    6,    0,   79,   79,   79,    0,    0,   12,   13,
   14,   15,   16,   17,   74,   75,    0,   74,   75,    0,
    0,   98,   68,   66,  140,   67,    0,   69,   74,   75,
   15,   16,   17,   15,   16,   17,   68,   66,    0,   67,
    0,   69,    0,    0,   15,   16,   17,   74,   75,    0,
   68,   66,    0,   67,  163,   69,   96,   68,   66,    0,
   67,    0,   69,   15,   16,   17,  115,   68,   66,    0,
   67,  101,   69,    0,   70,    0,   56,   57,    0,    3,
    4,    5,    6,    0,    0,  101,    0,   26,   70,   12,
   13,   14,    0,    0,   26,   68,   66,   43,   67,  101,
   69,    0,   70,  100,    0,    0,    0,    0,   58,   70,
   60,    0,    0,    0,   65,    0,    0,  100,   26,   70,
   71,   72,    0,    0,   76,    0,    0,    0,    0,   78,
   79,  100,   26,    0,   83,   84,  116,   68,   66,    0,
   67,    0,   69,    0,    0,    0,    0,   70,   26,  103,
    0,  104,    0,    0,    0,  105,  106,  107,  108,  109,
  110,  134,    0,  113,    0,  169,   68,   66,  141,   67,
    0,   69,  119,    0,  126,  122,    0,    0,    0,  170,
   68,   66,  148,   67,    0,   69,  136,  137,  138,   70,
    0,    0,    0,    0,    0,   68,   66,    0,   67,    0,
   69,  142,  143,  162,    0,  164,  166,  146,  147,    0,
  149,  150,  171,  172,   82,    0,    0,    0,   70,    0,
    0,    0,  183,    0,    0,    0,  185,    0,  186,    0,
  188,    0,   70,    0,  167,    0,  189,   74,   75,    0,
    0,  173,  178,  179,  165,  197,  184,   70,    0,    0,
  199,   74,   75,   15,   16,   17,    0,    0,  187,    0,
    0,    0,  191,    0,  193,   74,   75,   15,   16,   17,
   90,   90,   74,   90,    0,   90,    0,    0,    0,    0,
    0,   15,   16,   17,    0,    0,   74,   68,   66,   90,
   67,    0,   69,   68,   66,    0,   67,    0,   69,   68,
   66,    0,   67,    0,   69,    0,  111,    0,   68,   66,
    0,   67,  112,   69,   68,   66,    0,   67,  120,   69,
    0,    0,   90,   68,   66,    0,   67,  121,   69,    0,
    0,    0,    0,  144,    0,    0,    0,    0,    0,   70,
    0,    0,  151,    0,    0,   70,    0,    0,    0,    0,
    0,   70,    0,    0,    0,    0,    0,    0,    0,    0,
   70,    0,    0,    0,    0,    0,   70,    0,    0,    0,
    0,    0,    0,    0,    0,   70,
};
static const YYINT yycheck[] = {                         40,
    0,   40,  257,   43,   45,   45,   47,   35,    0,   53,
   54,   55,   40,   40,    0,  270,  257,  272,   40,   40,
   40,    0,    0,   45,   91,   47,    0,    0,    0,    0,
  257,   40,  273,    0,    0,  276,   40,    0,    0,    0,
  257,   41,   42,   43,  271,   45,   40,   47,    0,   41,
   42,   43,    0,   45,   40,   47,  123,    0,    0,   59,
    0,   61,   41,   42,   43,    0,   45,   59,   47,   61,
   41,   42,   43,   59,   45,   41,   47,   43,   91,   45,
   59,   59,   61,    0,    0,   59,   59,   59,   59,   41,
   61,   91,   59,   59,   94,   61,   59,   59,   59,   91,
    0,   41,   94,   43,   40,   45,   58,   59,   40,    0,
  123,   59,   91,   45,   40,    0,   59,   59,    0,   59,
   91,   61,  257,  123,   59,   91,    0,   40,    0,    0,
  257,  123,   42,   43,    0,   45,    0,   47,   59,    0,
    0,   93,   59,   59,  123,    0,  273,  274,  275,  257,
   58,   91,  123,   42,   43,   44,   45,  123,   47,   59,
   44,  257,   42,   43,  257,   45,  257,   47,   59,   58,
   40,   61,   61,  125,   59,   61,   41,   59,   91,   47,
   53,   91,   55,  123,   94,   59,   44,   59,   59,   40,
  257,  258,   44,   59,   45,   59,  273,  264,   59,   59,
   41,   41,   41,   41,   59,   94,  273,  274,  275,   41,
  123,   91,   41,  123,   94,  264,  257,  258,  257,  260,
  261,  262,  263,  264,  264,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  257,  258,  257,  260,  261,
  262,  263,   35,  123,  257,  258,    7,  269,  270,  271,
  272,  273,  274,  275,   42,   40,   40,  257,  258,   47,
  273,  274,  275,   94,  264,  257,  258,   41,   40,  125,
   31,   41,  264,  273,  274,  275,   93,  257,  257,  258,
  264,  273,  274,  275,   45,  264,  257,  258,  264,  264,
   41,  257,  258,  264,  273,  274,  275,   41,  264,   41,
  264,   41,  273,  274,  275,   41,   94,  273,  274,  275,
  264,  264,  264,  264,  264,  264,   40,  257,  258,   86,
   61,  158,    7,   54,  264,  257,  258,   -1,  260,  261,
  262,  263,   -1,  273,  274,  275,   -1,   -1,  270,  271,
  272,  273,  274,  275,  257,  258,   -1,  257,  258,   -1,
   -1,  264,   42,   43,  264,   45,   -1,   47,  257,  258,
  273,  274,  275,  273,  274,  275,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,  273,  274,  275,  257,  258,   -1,
   42,   43,   -1,   45,  264,   47,   41,   42,   43,   -1,
   45,   -1,   47,  273,  274,  275,   41,   42,   43,   -1,
   45,   91,   47,   -1,   94,   -1,  257,  258,   -1,  260,
  261,  262,  263,   -1,   -1,   91,   -1,    0,   94,  270,
  271,  272,   -1,   -1,    7,   42,   43,    7,   45,   91,
   47,   -1,   94,  123,   -1,   -1,   -1,   -1,   18,   94,
   20,   -1,   -1,   -1,   61,   -1,   -1,  123,   31,   94,
   30,   31,   -1,   -1,   37,   -1,   -1,   -1,   -1,   39,
   40,  123,   45,   -1,   44,   45,   41,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   94,   61,   62,
   -1,   64,   -1,   -1,   -1,   65,   66,   67,   68,   69,
   70,   98,   -1,   73,   -1,   41,   42,   43,  105,   45,
   -1,   47,   82,   -1,   87,   85,   -1,   -1,   -1,   41,
   42,   43,  119,   45,   -1,   47,   99,  100,  101,   94,
   -1,   -1,   -1,   -1,   -1,   42,   43,   -1,   45,   -1,
   47,  111,  112,  140,   -1,  142,  143,  117,  118,   -1,
  120,  121,  149,  150,   61,   -1,   -1,   -1,   94,   -1,
   -1,   -1,  159,   -1,   -1,   -1,  163,   -1,  165,   -1,
  167,   -1,   94,   -1,  144,   -1,  173,  257,  258,   -1,
   -1,  151,  155,  156,  264,  182,  159,   94,   -1,   -1,
  187,  257,  258,  273,  274,  275,   -1,   -1,  264,   -1,
   -1,   -1,  175,   -1,  177,  257,  258,  273,  274,  275,
   42,   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,  273,  274,  275,   -1,   -1,   58,   42,   43,   61,
   45,   -1,   47,   42,   43,   -1,   45,   -1,   47,   42,
   43,   -1,   45,   -1,   47,   -1,   61,   -1,   42,   43,
   -1,   45,   61,   47,   42,   43,   -1,   45,   61,   47,
   -1,   -1,   94,   42,   43,   -1,   45,   61,   47,   -1,
   -1,   -1,   -1,   61,   -1,   -1,   -1,   -1,   -1,   94,
   -1,   -1,   61,   -1,   -1,   94,   -1,   -1,   -1,   -1,
   -1,   94,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   94,   -1,   -1,   -1,   -1,   -1,   94,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   94,
};
#define YYFINAL 21
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
"eqn : STRING expr '=' expr",
"eqn : expr '=' expr attr",
"eqn : expr '=' expr STRING attr",
"eqn : STRING expr '=' expr attr",
"eqn : qual expr '=' expr",
"eqn : qual expr '=' expr STRING",
"eqn : STRING qual expr '=' expr",
"eqn : qual expr '=' expr attr",
"eqn : qual expr '=' expr STRING attr",
"eqn : STRING qual expr '=' expr attr",
"eqn : eqname expr '=' expr",
"eqn : eqname expr '=' expr STRING",
"eqn : STRING eqname expr '=' expr",
"eqn : eqname expr '=' expr attr",
"eqn : eqname expr '=' expr STRING attr",
"eqn : STRING eqname expr '=' expr attr",
"eqn : eqname qual expr '=' expr",
"eqn : eqname qual expr '=' expr STRING",
"eqn : STRING eqname qual expr '=' expr",
"eqn : eqname qual expr '=' expr attr",
"eqn : eqname qual expr '=' expr STRING attr",
"eqn : STRING eqname qual expr '=' expr attr",
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
#line 275 "parse.y"



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
#line 539 "parse.c"

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
#line 141 "parse.y"
	{ neweqn( 0,yystack.l_mark[-2],yystack.l_mark[0], 0, 0, 0); }
break;
case 42:
#line 142 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1], 0, 0,yystack.l_mark[0]); }
break;
case 43:
#line 143 "parse.y"
	{ neweqn( 0,yystack.l_mark[-2],yystack.l_mark[0], 0, 0,yystack.l_mark[-3]); }
break;
case 44:
#line 144 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0], 0, 0); }
break;
case 45:
#line 145 "parse.y"
	{ neweqn( 0,yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-1]); }
break;
case 46:
#line 146 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0], 0,yystack.l_mark[-4]); }
break;
case 47:
#line 147 "parse.y"
	{ neweqn(yystack.l_mark[-3],yystack.l_mark[-2],yystack.l_mark[0], 0, 0, 0); }
break;
case 48:
#line 148 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0, 0,yystack.l_mark[0]); }
break;
case 49:
#line 149 "parse.y"
	{ neweqn(yystack.l_mark[-3],yystack.l_mark[-2],yystack.l_mark[0], 0, 0,yystack.l_mark[-4]); }
break;
case 50:
#line 150 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0], 0, 0); }
break;
case 51:
#line 151 "parse.y"
	{ neweqn(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-1]); }
break;
case 52:
#line 152 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0], 0,yystack.l_mark[-5]); }
break;
case 53:
#line 153 "parse.y"
	{ neweqn( 0,yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-3], 0); }
break;
case 54:
#line 154 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1], 0,yystack.l_mark[-4],yystack.l_mark[0]); }
break;
case 55:
#line 155 "parse.y"
	{ neweqn( 0,yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-3],yystack.l_mark[-4]); }
break;
case 56:
#line 156 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0],yystack.l_mark[-4], 0); }
break;
case 57:
#line 157 "parse.y"
	{ neweqn( 0,yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0],yystack.l_mark[-5],yystack.l_mark[-1]); }
break;
case 58:
#line 158 "parse.y"
	{ neweqn( 0,yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0],yystack.l_mark[-4],yystack.l_mark[-5]); }
break;
case 59:
#line 159 "parse.y"
	{ neweqn(yystack.l_mark[-3],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-4], 0); }
break;
case 60:
#line 160 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1], 0,yystack.l_mark[-5],yystack.l_mark[0]); }
break;
case 61:
#line 161 "parse.y"
	{ neweqn(yystack.l_mark[-3],yystack.l_mark[-2],yystack.l_mark[0], 0,yystack.l_mark[-4],yystack.l_mark[-5]); }
break;
case 62:
#line 162 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0],yystack.l_mark[-5], 0); }
break;
case 63:
#line 163 "parse.y"
	{ neweqn(yystack.l_mark[-5],yystack.l_mark[-4],yystack.l_mark[-2],yystack.l_mark[0],yystack.l_mark[-6],yystack.l_mark[-1]); }
break;
case 64:
#line 164 "parse.y"
	{ neweqn(yystack.l_mark[-4],yystack.l_mark[-3],yystack.l_mark[-1],yystack.l_mark[0],yystack.l_mark[-5],yystack.l_mark[-6]); }
break;
case 65:
#line 167 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 66:
#line 168 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 67:
#line 171 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 68:
#line 174 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 69:
#line 175 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 70:
#line 176 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 71:
#line 179 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 72:
#line 180 "parse.y"
	{ yyval = yystack.l_mark[-2]; yystack.l_mark[-2]->r=yystack.l_mark[0]; }
break;
case 78:
#line 208 "parse.y"
	{ yyval = newnode(add, "+"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 79:
#line 209 "parse.y"
	{ yyval = newnode(sub, "-"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 80:
#line 210 "parse.y"
	{ yyval = newnode(mul, "*"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 81:
#line 211 "parse.y"
	{ yyval = newnode(dvd, "/"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 82:
#line 212 "parse.y"
	{ yyval = newnode(pow, "^"  ,yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 83:
#line 213 "parse.y"
	{ yyval = newnode(neg, "-"  , 0,yystack.l_mark[0]); }
break;
case 84:
#line 214 "parse.y"
	{ yyval = yystack.l_mark[-1]; }
break;
case 85:
#line 215 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 86:
#line 216 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 87:
#line 217 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 88:
#line 218 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 89:
#line 219 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 90:
#line 220 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 91:
#line 233 "parse.y"
	{ yyval = yystack.l_mark[0]; }
break;
case 92:
#line 234 "parse.y"
	{ yyval = yystack.l_mark[-3]; yystack.l_mark[-3]->r=newnode(lst,"lst",0,yystack.l_mark[-1]); }
break;
case 93:
#line 235 "parse.y"
	{ yyval = newnode(dom,"#",yystack.l_mark[-2],yystack.l_mark[0]); }
break;
case 94:
#line 236 "parse.y"
	{ yyval = newnode(dom,"#",yystack.l_mark[-5],yystack.l_mark[0]); yystack.l_mark[-5]->r=newnode(lst,"lst",0,yystack.l_mark[-3]); }
break;
case 95:
#line 250 "parse.y"
	{ yyval = newnode(log,"log", 0,yystack.l_mark[-1]); }
break;
case 96:
#line 251 "parse.y"
	{ yyval = newnode(exp,"exp", 0,yystack.l_mark[-1]); }
break;
case 97:
#line 254 "parse.y"
	{ yyval = newnode(lag,"lag", 0,yystack.l_mark[-1]); }
break;
case 98:
#line 255 "parse.y"
	{ yyval = newnode(lag,"lag", 0,yystack.l_mark[-1]); }
break;
case 99:
#line 258 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 100:
#line 259 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 101:
#line 260 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 102:
#line 261 "parse.y"
	{ yyval = newnode(led,"lead",0,yystack.l_mark[-1]); }
break;
case 103:
#line 264 "parse.y"
	{ yyval = newnode(sum,"sum"  ,yystack.l_mark[-3],yystack.l_mark[-1]); }
break;
case 104:
#line 265 "parse.y"
	{ yyval = newnode(prd,"prod" ,yystack.l_mark[-3],yystack.l_mark[-1]); }
break;
#line 1086 "parse.c"
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
