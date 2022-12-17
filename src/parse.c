/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAME = 258,
     NUM = 259,
     NEG = 260,
     LOG = 261,
     EXP = 262,
     SUM = 263,
     PROD = 264,
     STRING = 265,
     BADNUM = 266,
     SET = 267,
     VAR = 268,
     PAR = 269,
     EQU = 270,
     LEAD = 271,
     LAG = 272,
     NEXT = 273,
     TIME = 274,
     FIRST = 275,
     LAST = 276,
     UNION = 277
   };
#endif
/* Tokens.  */
#define NAME 258
#define NUM 259
#define NEG 260
#define LOG 261
#define EXP 262
#define SUM 263
#define PROD 264
#define STRING 265
#define BADNUM 266
#define SET 267
#define VAR 268
#define PAR 269
#define EQU 270
#define LEAD 271
#define LAG 272
#define NEXT 273
#define TIME 274
#define FIRST 275
#define LAST 276
#define UNION 277




/* Copy the first part of user declarations.  */
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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 173 "parse.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  61
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   436

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNRULES -- Number of states.  */
#define YYNSTATES  208

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   277

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    38,     2,     2,     2,     2,
      31,    32,    27,    25,    23,    26,     2,    28,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    33,    30,
       2,    24,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    36,     2,    37,    29,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,    35,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     9,    10,    12,    14,    17,    21,
      25,    30,    35,    41,    48,    55,    63,    71,    73,    75,
      77,    83,    90,    95,   101,   109,   118,   126,   135,   144,
     154,   163,   173,   180,   188,   195,   203,   211,   220,   222,
     224,   226,   228,   232,   237,   242,   247,   253,   259,   264,
     270,   276,   282,   289,   296,   301,   307,   313,   319,   326,
     333,   339,   346,   353,   360,   368,   376,   380,   383,   386,
     388,   392,   396,   398,   402,   404,   406,   408,   410,   412,
     416,   420,   424,   428,   432,   435,   439,   441,   443,   445,
     447,   449,   451,   453,   458,   462,   469,   474,   479,   484,
     489,   494,   499,   504,   509,   516
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      40,     0,    -1,    41,    -1,    40,    30,    41,    -1,    -1,
      42,    -1,    47,    -1,    43,     3,    -1,    43,     3,    10,
      -1,    43,     3,    50,    -1,    43,     3,    10,    50,    -1,
      43,     3,    51,    10,    -1,    43,     3,    31,    51,    32,
      -1,    43,     3,    31,    51,    32,    10,    -1,    43,     3,
      31,    51,    32,    50,    -1,    43,     3,    31,    51,    32,
      10,    50,    -1,    43,     3,    31,    51,    32,    51,    10,
      -1,    44,    -1,    14,    -1,    13,    -1,    12,    45,    31,
      51,    32,    -1,    12,    45,    31,    51,    32,    10,    -1,
      12,     3,    24,     3,    -1,    12,     3,    24,     3,    10,
      -1,    12,     3,    24,    45,    31,    51,    32,    -1,    12,
       3,    24,    45,    31,    51,    32,    10,    -1,    12,    46,
      24,    19,    31,    52,    32,    -1,    12,    46,    24,    19,
      31,    52,    32,    10,    -1,    12,     3,    24,     3,    25,
      31,    51,    32,    -1,    12,     3,    24,     3,    25,    31,
      51,    32,    10,    -1,    12,     3,    24,     3,    26,    31,
      51,    32,    -1,    12,     3,    24,     3,    26,    31,    51,
      32,    10,    -1,    12,     3,    24,     3,    25,     3,    -1,
      12,     3,    24,     3,    25,     3,    10,    -1,    12,     3,
      24,     3,    26,     3,    -1,    12,     3,    24,     3,    26,
       3,    10,    -1,    12,     3,    24,    22,    31,    51,    32,
      -1,    12,     3,    24,    22,    31,    51,    32,    10,    -1,
       3,    -1,    19,    -1,    20,    -1,    21,    -1,    53,    24,
      53,    -1,    53,    24,    53,    10,    -1,    10,    53,    24,
      53,    -1,    53,    24,    53,    50,    -1,    53,    24,    53,
      10,    50,    -1,    10,    53,    24,    53,    50,    -1,    49,
      53,    24,    53,    -1,    49,    53,    24,    53,    10,    -1,
      10,    49,    53,    24,    53,    -1,    49,    53,    24,    53,
      50,    -1,    49,    53,    24,    53,    10,    50,    -1,    10,
      49,    53,    24,    53,    50,    -1,    48,    53,    24,    53,
      -1,    48,    53,    24,    53,    10,    -1,    10,    48,    53,
      24,    53,    -1,    48,    53,    24,    53,    50,    -1,    48,
      53,    24,    53,    10,    50,    -1,    10,    48,    53,    24,
      53,    50,    -1,    48,    49,    53,    24,    53,    -1,    48,
      49,    53,    24,    53,    10,    -1,    10,    48,    49,    53,
      24,    53,    -1,    48,    49,    53,    24,    53,    50,    -1,
      48,    49,    53,    24,    53,    10,    50,    -1,    10,    48,
      49,    53,    24,    53,    50,    -1,    28,     3,    28,    -1,
      15,     3,    -1,    51,    33,    -1,    51,    -1,    34,    51,
      35,    -1,    36,    51,    37,    -1,    52,    -1,    52,    23,
      51,    -1,     3,    -1,     4,    -1,    20,    -1,    21,    -1,
      19,    -1,    53,    25,    53,    -1,    53,    26,    53,    -1,
      53,    27,    53,    -1,    53,    28,    53,    -1,    53,    29,
      53,    -1,    26,    53,    -1,    31,    53,    32,    -1,    55,
      -1,    58,    -1,    56,    -1,    57,    -1,    54,    -1,     4,
      -1,     3,    -1,     3,    31,    51,    32,    -1,     3,    38,
       3,    -1,     3,    31,    51,    32,    38,     3,    -1,     6,
      31,    53,    32,    -1,     7,    31,    53,    32,    -1,    17,
      31,    54,    32,    -1,    17,    31,    56,    32,    -1,    16,
      31,    54,    32,    -1,    16,    31,    57,    32,    -1,    18,
      31,    54,    32,    -1,    18,    31,    57,    32,    -1,     8,
      31,     3,    23,    53,    32,    -1,     9,    31,     3,    23,
      53,    32,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    53,    53,    54,    57,    58,    59,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,   100,   101,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   125,   126,
     129,   130,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   167,   168,   171,   174,
     175,   176,   179,   180,   183,   184,   185,   186,   187,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   233,   234,   235,   236,   250,   251,   254,   255,
     258,   259,   260,   261,   264,   265
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "NUM", "NEG", "LOG", "EXP",
  "SUM", "PROD", "STRING", "BADNUM", "SET", "VAR", "PAR", "EQU", "LEAD",
  "LAG", "NEXT", "TIME", "FIRST", "LAST", "UNION", "','", "'='", "'+'",
  "'-'", "'*'", "'/'", "'^'", "';'", "'('", "')'", "':'", "'{'", "'}'",
  "'['", "']'", "'#'", "$accept", "file", "stmt", "declare", "decl",
  "decset", "tiok", "forl", "eqn", "eqname", "qual", "attr", "list",
  "item", "expr", "var", "func", "lag", "lead", "setfunc", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    44,    61,    43,    45,    42,    47,    94,
      59,    40,    41,    58,   123,   125,    91,    93,    35
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    40,    41,    41,    41,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    43,    43,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    45,    45,
      46,    46,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    48,    48,    49,    50,
      50,    50,    51,    51,    52,    52,    52,    52,    52,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    54,    54,    54,    54,    55,    55,    56,    56,
      57,    57,    57,    57,    58,    58
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     0,     1,     1,     2,     3,     3,
       4,     4,     5,     6,     6,     7,     7,     1,     1,     1,
       5,     6,     4,     5,     7,     8,     7,     8,     8,     9,
       8,     9,     6,     7,     6,     7,     7,     8,     1,     1,
       1,     1,     3,     4,     4,     4,     5,     5,     4,     5,
       5,     5,     6,     6,     4,     5,     5,     5,     6,     6,
       5,     6,     6,     6,     7,     7,     3,     2,     2,     1,
       3,     3,     1,     3,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     2,     3,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     6,     4,     4,     4,     4,
       4,     4,     4,     4,     6,     6
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,    92,    91,     0,     0,     0,     0,     0,     0,    19,
      18,     0,     0,     0,     0,    78,    76,    77,     0,     0,
       0,     0,     2,     5,     0,    17,     6,     0,     0,     0,
      72,     0,    90,    86,    88,    89,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    38,    39,    40,    41,
       0,     0,    67,     0,     0,     0,    92,    91,    84,     0,
       0,     1,     4,     7,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,    74,    75,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    85,     3,     8,
       0,     0,     0,     9,    69,     0,     0,     0,    73,    42,
      79,    80,    81,    82,    83,    93,    96,    97,     0,     0,
       0,     0,     0,    44,    22,     0,     0,     0,     0,   100,
     101,    98,    99,   102,   103,    10,    69,     0,     0,     0,
      11,     0,    54,    48,    43,    45,     0,     0,     0,     0,
      56,    50,    47,    23,     0,     0,     0,     0,    20,     0,
      12,    70,    71,    60,    55,    57,    49,    51,    46,    95,
     104,   105,    62,    59,    53,    32,     0,    34,     0,     0,
       0,    21,     0,    13,    14,    69,    61,    63,    58,    52,
      65,    33,     0,    35,     0,    36,    24,    26,    15,    16,
      64,    28,    30,    37,    25,    27,    29,    31
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    21,    22,    23,    24,    25,    50,    51,    26,    27,
      28,   103,   136,    30,    31,    32,    33,    34,    35,    36
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -50
static const yytype_int16 yypact[] =
{
     274,    80,   -20,   -21,   -16,   -13,    38,   300,    33,   -50,
     -50,    28,    50,    52,    54,   -50,   -50,   -50,   347,    29,
     347,     5,   -50,   -50,    83,   -50,   -50,   326,   347,    56,
      67,   312,   -50,   -50,   -50,   -50,   -50,    95,    91,   347,
     347,   101,   102,   326,   347,   371,    85,   -50,   -50,   -50,
      90,   100,   -50,   114,     9,   114,   -15,   -50,   -50,   103,
     112,   -50,   274,   240,   347,   377,   383,   -50,    95,   347,
     347,   347,   347,   347,   347,   -50,   -50,    96,   -50,   237,
     342,   106,   113,   347,   389,   395,   347,    -2,    95,   116,
     122,   126,   127,   129,   130,   133,   -50,   -50,   -50,    76,
      95,    95,    95,   -50,   135,   401,   347,   347,   -50,   160,
      19,    19,   119,   119,   -50,   134,   -50,   -50,   347,   347,
     407,   347,   347,    30,    -1,   136,   137,   139,   142,   -50,
     -50,   -50,   -50,   -50,   -50,   -50,   -50,   143,   147,   140,
     -50,   347,   180,   200,    76,   -50,   171,   354,   362,   347,
      30,    30,   -50,   -50,     8,    11,    95,    95,   181,    95,
      57,   -50,   -50,   220,    76,   -50,    76,   -50,   -50,   -50,
     -50,   -50,    30,   -50,   -50,   182,    95,   183,    95,   163,
     165,   -50,   166,    76,   -50,   192,    76,   -50,   -50,   -50,
     -50,   -50,   179,   -50,   185,   202,   203,   205,   -50,   -50,
     -50,   208,   212,   -50,   -50,   -50,   -50,   -50
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -50,   -50,   169,   -50,   -50,   -50,   145,   -50,   -50,   226,
      -5,   -17,     0,    78,     1,    98,   -50,   184,   -49,   -50
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -76
static const yytype_int16 yytable[] =
{
      29,   124,    44,   -75,    91,    61,    95,    29,    45,   153,
      39,   175,    56,   -75,   177,    40,    37,    47,    41,    58,
     125,    60,    64,    38,   154,   155,    13,    29,    65,    66,
     -38,    52,    59,    75,    76,    62,    46,    77,    83,   176,
      79,    80,   178,    29,    84,    85,    72,    73,    74,    15,
      16,    17,    47,    48,    49,    70,    71,    72,    73,    74,
      75,    76,    29,   104,   101,   105,   102,   183,   108,    42,
     109,   110,   111,   112,   113,   114,    15,    16,    17,    75,
      76,    53,   135,    54,   120,    55,    63,   123,   127,    67,
      68,   101,   145,   102,    78,    15,    16,    17,    75,    76,
     137,   138,   139,   -74,    81,    82,   152,   142,   143,    87,
     101,    37,   102,   -74,    15,    16,    17,    56,    38,   147,
     148,    88,   150,   151,    89,   165,   167,   168,   115,   118,
      12,    96,    14,   173,   174,   128,   119,    70,    71,    72,
      73,    74,   163,   184,    97,   140,   187,   188,    74,   189,
     172,    90,    92,    94,   129,   190,   179,   180,   130,   131,
     185,   132,   133,    75,    76,   134,   198,   156,   157,   200,
     144,   158,   146,   159,   169,   160,   192,   162,   194,    15,
      16,    17,   161,    75,    76,    70,    71,    72,    73,    74,
     164,   181,   191,   193,   101,   195,   102,   196,   197,    15,
      16,    17,   199,    75,    76,    70,    71,    72,    73,    74,
     166,   201,   203,   204,   101,   205,   102,   202,   206,    15,
      16,    17,   207,    75,    76,    70,    71,    72,    73,    74,
     186,    98,   126,    43,   101,     0,   102,   182,    93,    15,
      16,    17,     0,    75,    76,    70,    71,    72,    73,    74,
      99,     0,     0,     0,   101,     0,   102,     0,     0,    15,
      16,    17,    70,    71,    72,    73,    74,     0,     0,   116,
       0,   100,     0,     0,   101,     0,   102,     1,     2,     0,
       3,     4,     5,     6,     7,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,     0,     0,     0,     0,
      18,     0,    19,     1,     2,    20,     3,     4,     5,     6,
       0,     0,     0,     0,     0,    11,    12,    13,    14,    15,
      16,    17,     0,     0,     0,     0,    18,     0,    19,     1,
       2,    20,     3,     4,     5,     6,    69,    70,    71,    72,
      73,    74,    12,    13,    14,    15,    16,    17,     0,     0,
      56,    57,    18,     3,     4,     5,     6,    20,     0,     0,
       0,     0,     0,    12,    13,    14,     0,    70,    71,    72,
      73,    74,     0,    18,   117,     0,     0,     0,    20,    70,
      71,    72,    73,    74,     0,     0,   170,    70,    71,    72,
      73,    74,     0,     0,   171,    86,    70,    71,    72,    73,
      74,   106,    70,    71,    72,    73,    74,   107,    70,    71,
      72,    73,    74,   121,    70,    71,    72,    73,    74,   122,
      70,    71,    72,    73,    74,   141,    70,    71,    72,    73,
      74,   149,    70,    71,    72,    73,    74
};

static const yytype_int16 yycheck[] =
{
       0,     3,     7,    23,    53,     0,    55,     7,     7,    10,
      31,     3,     3,    33,     3,    31,    31,    19,    31,    18,
      22,    20,    27,    38,    25,    26,    17,    27,    27,    28,
      31,     3,     3,     3,     4,    30,     3,    37,    43,    31,
      39,    40,    31,    43,    43,    44,    27,    28,    29,    19,
      20,    21,    19,    20,    21,    25,    26,    27,    28,    29,
       3,     4,    62,    63,    34,    64,    36,    10,    68,    31,
      69,    70,    71,    72,    73,    74,    19,    20,    21,     3,
       4,    31,    99,    31,    83,    31,     3,    86,    88,    33,
      23,    34,   109,    36,     3,    19,    20,    21,     3,     4,
     100,   101,   102,    23,     3,     3,   123,   106,   107,    24,
      34,    31,    36,    33,    19,    20,    21,     3,    38,   118,
     119,    31,   121,   122,    24,   142,   143,   144,    32,    23,
      16,    28,    18,   150,   151,    19,    23,    25,    26,    27,
      28,    29,   141,   160,    32,    10,   163,   164,    29,   166,
     149,    53,    54,    55,    32,   172,   156,   157,    32,    32,
     160,    32,    32,     3,     4,    32,   183,    31,    31,   186,
      10,    32,    38,    31,     3,    32,   176,    37,   178,    19,
      20,    21,    35,     3,     4,    25,    26,    27,    28,    29,
      10,    10,    10,    10,    34,    32,    36,    32,    32,    19,
      20,    21,    10,     3,     4,    25,    26,    27,    28,    29,
      10,    32,    10,    10,    34,    10,    36,    32,    10,    19,
      20,    21,    10,     3,     4,    25,    26,    27,    28,    29,
      10,    62,    87,     7,    34,    -1,    36,   159,    54,    19,
      20,    21,    -1,     3,     4,    25,    26,    27,    28,    29,
      10,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    19,
      20,    21,    25,    26,    27,    28,    29,    -1,    -1,    32,
      -1,    31,    -1,    -1,    34,    -1,    36,     3,     4,    -1,
       6,     7,     8,     9,    10,    -1,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      26,    -1,    28,     3,     4,    31,     6,     7,     8,     9,
      -1,    -1,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    26,    -1,    28,     3,
       4,    31,     6,     7,     8,     9,    24,    25,    26,    27,
      28,    29,    16,    17,    18,    19,    20,    21,    -1,    -1,
       3,     4,    26,     6,     7,     8,     9,    31,    -1,    -1,
      -1,    -1,    -1,    16,    17,    18,    -1,    25,    26,    27,
      28,    29,    -1,    26,    32,    -1,    -1,    -1,    31,    25,
      26,    27,    28,    29,    -1,    -1,    32,    25,    26,    27,
      28,    29,    -1,    -1,    32,    24,    25,    26,    27,    28,
      29,    24,    25,    26,    27,    28,    29,    24,    25,    26,
      27,    28,    29,    24,    25,    26,    27,    28,    29,    24,
      25,    26,    27,    28,    29,    24,    25,    26,    27,    28,
      29,    24,    25,    26,    27,    28,    29
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     6,     7,     8,     9,    10,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    26,    28,
      31,    40,    41,    42,    43,    44,    47,    48,    49,    51,
      52,    53,    54,    55,    56,    57,    58,    31,    38,    31,
      31,    31,    31,    48,    49,    53,     3,    19,    20,    21,
      45,    46,     3,    31,    31,    31,     3,     4,    53,     3,
      53,     0,    30,     3,    49,    53,    53,    33,    23,    24,
      25,    26,    27,    28,    29,     3,     4,    51,     3,    53,
      53,     3,     3,    49,    53,    53,    24,    24,    31,    24,
      54,    57,    54,    56,    54,    57,    28,    32,    41,    10,
      31,    34,    36,    50,    51,    53,    24,    24,    51,    53,
      53,    53,    53,    53,    53,    32,    32,    32,    23,    23,
      53,    24,    24,    53,     3,    22,    45,    51,    19,    32,
      32,    32,    32,    32,    32,    50,    51,    51,    51,    51,
      10,    24,    53,    53,    10,    50,    38,    53,    53,    24,
      53,    53,    50,    10,    25,    26,    31,    31,    32,    31,
      32,    35,    37,    53,    10,    50,    10,    50,    50,     3,
      32,    32,    53,    50,    50,     3,    31,     3,    31,    51,
      51,    10,    52,    10,    50,    51,    10,    50,    50,    50,
      50,    10,    51,    10,    51,    32,    32,    32,    50,    10,
      50,    32,    32,    10,    10,    10,    10,    10
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 7:
#line 87 "parse.y"
    { declare((yyvsp[(1) - (2)]),(yyvsp[(2) - (2)]), 0, 0, 0); }
    break;

  case 8:
#line 88 "parse.y"
    { declare((yyvsp[(1) - (3)]),(yyvsp[(2) - (3)]), 0,(yyvsp[(3) - (3)]), 0); }
    break;

  case 9:
#line 89 "parse.y"
    { declare((yyvsp[(1) - (3)]),(yyvsp[(2) - (3)]), 0, 0,(yyvsp[(3) - (3)])); }
    break;

  case 10:
#line 90 "parse.y"
    { declare((yyvsp[(1) - (4)]),(yyvsp[(2) - (4)]), 0,(yyvsp[(3) - (4)]),(yyvsp[(4) - (4)])); }
    break;

  case 11:
#line 91 "parse.y"
    { declare((yyvsp[(1) - (4)]),(yyvsp[(2) - (4)]), 0,(yyvsp[(4) - (4)]),(yyvsp[(3) - (4)])); }
    break;

  case 12:
#line 92 "parse.y"
    { declare((yyvsp[(1) - (5)]),(yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]), 0, 0); }
    break;

  case 13:
#line 93 "parse.y"
    { declare((yyvsp[(1) - (6)]),(yyvsp[(2) - (6)]),(yyvsp[(4) - (6)]),(yyvsp[(6) - (6)]), 0); }
    break;

  case 14:
#line 94 "parse.y"
    { declare((yyvsp[(1) - (6)]),(yyvsp[(2) - (6)]),(yyvsp[(4) - (6)]), 0,(yyvsp[(6) - (6)])); }
    break;

  case 15:
#line 95 "parse.y"
    { declare((yyvsp[(1) - (7)]),(yyvsp[(2) - (7)]),(yyvsp[(4) - (7)]),(yyvsp[(6) - (7)]),(yyvsp[(7) - (7)])); }
    break;

  case 16:
#line 96 "parse.y"
    { declare((yyvsp[(1) - (7)]),(yyvsp[(2) - (7)]),(yyvsp[(4) - (7)]),(yyvsp[(7) - (7)]),(yyvsp[(6) - (7)])); }
    break;

  case 20:
#line 105 "parse.y"
    { declare((yyvsp[(1) - (5)]),(yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]), 0, 0) ; }
    break;

  case 21:
#line 106 "parse.y"
    { declare((yyvsp[(1) - (6)]),(yyvsp[(2) - (6)]),(yyvsp[(4) - (6)]),(yyvsp[(6) - (6)]), 0) ; }
    break;

  case 22:
#line 107 "parse.y"
    { decset ((yyvsp[(2) - (4)]),(yyvsp[(4) - (4)]),nul, 0, 0); }
    break;

  case 23:
#line 108 "parse.y"
    { decset ((yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),nul, 0,(yyvsp[(5) - (5)])); }
    break;

  case 24:
#line 109 "parse.y"
    { decset ((yyvsp[(2) - (7)]),(yyvsp[(4) - (7)]),equ,(yyvsp[(6) - (7)]), 0); }
    break;

  case 25:
#line 110 "parse.y"
    { decset ((yyvsp[(2) - (8)]),(yyvsp[(4) - (8)]),equ,(yyvsp[(6) - (8)]),(yyvsp[(8) - (8)])); }
    break;

  case 26:
#line 111 "parse.y"
    { decset ((yyvsp[(2) - (7)]),(yyvsp[(4) - (7)]),equ,(yyvsp[(6) - (7)]), 0); }
    break;

  case 27:
#line 112 "parse.y"
    { decset ((yyvsp[(2) - (8)]),(yyvsp[(4) - (8)]),equ,(yyvsp[(6) - (8)]),(yyvsp[(8) - (8)])); }
    break;

  case 28:
#line 113 "parse.y"
    { decset ((yyvsp[(2) - (8)]),(yyvsp[(4) - (8)]),add,(yyvsp[(7) - (8)]), 0); }
    break;

  case 29:
#line 114 "parse.y"
    { decset ((yyvsp[(2) - (9)]),(yyvsp[(4) - (9)]),add,(yyvsp[(7) - (9)]),(yyvsp[(9) - (9)])); }
    break;

  case 30:
#line 115 "parse.y"
    { decset ((yyvsp[(2) - (8)]),(yyvsp[(4) - (8)]),sub,(yyvsp[(7) - (8)]), 0); }
    break;

  case 31:
#line 116 "parse.y"
    { decset ((yyvsp[(2) - (9)]),(yyvsp[(4) - (9)]),sub,(yyvsp[(7) - (9)]),(yyvsp[(9) - (9)])); }
    break;

  case 32:
#line 117 "parse.y"
    { decset ((yyvsp[(2) - (6)]),(yyvsp[(4) - (6)]),sad,(yyvsp[(6) - (6)]), 0); }
    break;

  case 33:
#line 118 "parse.y"
    { decset ((yyvsp[(2) - (7)]),(yyvsp[(4) - (7)]),sad,(yyvsp[(6) - (7)]),(yyvsp[(7) - (7)])); }
    break;

  case 34:
#line 119 "parse.y"
    { decset ((yyvsp[(2) - (6)]),(yyvsp[(4) - (6)]),ssu,(yyvsp[(6) - (6)]), 0); }
    break;

  case 35:
#line 120 "parse.y"
    { decset ((yyvsp[(2) - (7)]),(yyvsp[(4) - (7)]),ssu,(yyvsp[(6) - (7)]),(yyvsp[(7) - (7)])); }
    break;

  case 36:
#line 121 "parse.y"
    { decunion((yyvsp[(2) - (7)]),(yyvsp[(6) - (7)]), 0); }
    break;

  case 37:
#line 122 "parse.y"
    { decunion((yyvsp[(2) - (8)]),(yyvsp[(6) - (8)]),(yyvsp[(7) - (8)])); }
    break;

  case 42:
#line 141 "parse.y"
    { neweqn( 0,(yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 0, 0, 0); }
    break;

  case 43:
#line 142 "parse.y"
    { neweqn( 0,(yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]), 0, 0,(yyvsp[(4) - (4)])); }
    break;

  case 44:
#line 143 "parse.y"
    { neweqn( 0,(yyvsp[(2) - (4)]),(yyvsp[(4) - (4)]), 0, 0,(yyvsp[(1) - (4)])); }
    break;

  case 45:
#line 144 "parse.y"
    { neweqn( 0,(yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),(yyvsp[(4) - (4)]), 0, 0); }
    break;

  case 46:
#line 145 "parse.y"
    { neweqn( 0,(yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),(yyvsp[(5) - (5)]), 0,(yyvsp[(4) - (5)])); }
    break;

  case 47:
#line 146 "parse.y"
    { neweqn( 0,(yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),(yyvsp[(5) - (5)]), 0,(yyvsp[(1) - (5)])); }
    break;

  case 48:
#line 147 "parse.y"
    { neweqn((yyvsp[(1) - (4)]),(yyvsp[(2) - (4)]),(yyvsp[(4) - (4)]), 0, 0, 0); }
    break;

  case 49:
#line 148 "parse.y"
    { neweqn((yyvsp[(1) - (5)]),(yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]), 0, 0,(yyvsp[(5) - (5)])); }
    break;

  case 50:
#line 149 "parse.y"
    { neweqn((yyvsp[(2) - (5)]),(yyvsp[(3) - (5)]),(yyvsp[(5) - (5)]), 0, 0,(yyvsp[(1) - (5)])); }
    break;

  case 51:
#line 150 "parse.y"
    { neweqn((yyvsp[(1) - (5)]),(yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),(yyvsp[(5) - (5)]), 0, 0); }
    break;

  case 52:
#line 151 "parse.y"
    { neweqn((yyvsp[(1) - (6)]),(yyvsp[(2) - (6)]),(yyvsp[(4) - (6)]),(yyvsp[(6) - (6)]), 0,(yyvsp[(5) - (6)])); }
    break;

  case 53:
#line 152 "parse.y"
    { neweqn((yyvsp[(2) - (6)]),(yyvsp[(3) - (6)]),(yyvsp[(5) - (6)]),(yyvsp[(6) - (6)]), 0,(yyvsp[(1) - (6)])); }
    break;

  case 54:
#line 153 "parse.y"
    { neweqn( 0,(yyvsp[(2) - (4)]),(yyvsp[(4) - (4)]), 0,(yyvsp[(1) - (4)]), 0); }
    break;

  case 55:
#line 154 "parse.y"
    { neweqn( 0,(yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]), 0,(yyvsp[(1) - (5)]),(yyvsp[(5) - (5)])); }
    break;

  case 56:
#line 155 "parse.y"
    { neweqn( 0,(yyvsp[(3) - (5)]),(yyvsp[(5) - (5)]), 0,(yyvsp[(2) - (5)]),(yyvsp[(1) - (5)])); }
    break;

  case 57:
#line 156 "parse.y"
    { neweqn( 0,(yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),(yyvsp[(5) - (5)]),(yyvsp[(1) - (5)]), 0); }
    break;

  case 58:
#line 157 "parse.y"
    { neweqn( 0,(yyvsp[(2) - (6)]),(yyvsp[(4) - (6)]),(yyvsp[(6) - (6)]),(yyvsp[(1) - (6)]),(yyvsp[(5) - (6)])); }
    break;

  case 59:
#line 158 "parse.y"
    { neweqn( 0,(yyvsp[(3) - (6)]),(yyvsp[(5) - (6)]),(yyvsp[(6) - (6)]),(yyvsp[(2) - (6)]),(yyvsp[(1) - (6)])); }
    break;

  case 60:
#line 159 "parse.y"
    { neweqn((yyvsp[(2) - (5)]),(yyvsp[(3) - (5)]),(yyvsp[(5) - (5)]), 0,(yyvsp[(1) - (5)]), 0); }
    break;

  case 61:
#line 160 "parse.y"
    { neweqn((yyvsp[(2) - (6)]),(yyvsp[(3) - (6)]),(yyvsp[(5) - (6)]), 0,(yyvsp[(1) - (6)]),(yyvsp[(6) - (6)])); }
    break;

  case 62:
#line 161 "parse.y"
    { neweqn((yyvsp[(3) - (6)]),(yyvsp[(4) - (6)]),(yyvsp[(6) - (6)]), 0,(yyvsp[(2) - (6)]),(yyvsp[(1) - (6)])); }
    break;

  case 63:
#line 162 "parse.y"
    { neweqn((yyvsp[(2) - (6)]),(yyvsp[(3) - (6)]),(yyvsp[(5) - (6)]),(yyvsp[(6) - (6)]),(yyvsp[(1) - (6)]), 0); }
    break;

  case 64:
#line 163 "parse.y"
    { neweqn((yyvsp[(2) - (7)]),(yyvsp[(3) - (7)]),(yyvsp[(5) - (7)]),(yyvsp[(7) - (7)]),(yyvsp[(1) - (7)]),(yyvsp[(6) - (7)])); }
    break;

  case 65:
#line 164 "parse.y"
    { neweqn((yyvsp[(3) - (7)]),(yyvsp[(4) - (7)]),(yyvsp[(6) - (7)]),(yyvsp[(7) - (7)]),(yyvsp[(2) - (7)]),(yyvsp[(1) - (7)])); }
    break;

  case 66:
#line 167 "parse.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 67:
#line 168 "parse.y"
    { (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 68:
#line 171 "parse.y"
    { (yyval) = (yyvsp[(1) - (2)]); }
    break;

  case 69:
#line 174 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 70:
#line 175 "parse.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 71:
#line 176 "parse.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 72:
#line 179 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 73:
#line 180 "parse.y"
    { (yyval) = (yyvsp[(1) - (3)]); (yyvsp[(1) - (3)])->r=(yyvsp[(3) - (3)]); }
    break;

  case 79:
#line 208 "parse.y"
    { (yyval) = newnode(add, "+"  ,(yyvsp[(1) - (3)]),(yyvsp[(3) - (3)])); }
    break;

  case 80:
#line 209 "parse.y"
    { (yyval) = newnode(sub, "-"  ,(yyvsp[(1) - (3)]),(yyvsp[(3) - (3)])); }
    break;

  case 81:
#line 210 "parse.y"
    { (yyval) = newnode(mul, "*"  ,(yyvsp[(1) - (3)]),(yyvsp[(3) - (3)])); }
    break;

  case 82:
#line 211 "parse.y"
    { (yyval) = newnode(dvd, "/"  ,(yyvsp[(1) - (3)]),(yyvsp[(3) - (3)])); }
    break;

  case 83:
#line 212 "parse.y"
    { (yyval) = newnode(pow, "^"  ,(yyvsp[(1) - (3)]),(yyvsp[(3) - (3)])); }
    break;

  case 84:
#line 213 "parse.y"
    { (yyval) = newnode(neg, "-"  , 0,(yyvsp[(2) - (2)])); }
    break;

  case 85:
#line 214 "parse.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 86:
#line 215 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 87:
#line 216 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 88:
#line 217 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 89:
#line 218 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 90:
#line 219 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 91:
#line 220 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 92:
#line 233 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 93:
#line 234 "parse.y"
    { (yyval) = (yyvsp[(1) - (4)]); (yyvsp[(1) - (4)])->r=newnode(lst,"lst",0,(yyvsp[(3) - (4)])); }
    break;

  case 94:
#line 235 "parse.y"
    { (yyval) = newnode(dom,"#",(yyvsp[(1) - (3)]),(yyvsp[(3) - (3)])); }
    break;

  case 95:
#line 236 "parse.y"
    { (yyval) = newnode(dom,"#",(yyvsp[(1) - (6)]),(yyvsp[(6) - (6)])); (yyvsp[(1) - (6)])->r=newnode(lst,"lst",0,(yyvsp[(3) - (6)])); }
    break;

  case 96:
#line 250 "parse.y"
    { (yyval) = newnode(log,"log", 0,(yyvsp[(3) - (4)])); }
    break;

  case 97:
#line 251 "parse.y"
    { (yyval) = newnode(exp,"exp", 0,(yyvsp[(3) - (4)])); }
    break;

  case 98:
#line 254 "parse.y"
    { (yyval) = newnode(lag,"lag", 0,(yyvsp[(3) - (4)])); }
    break;

  case 99:
#line 255 "parse.y"
    { (yyval) = newnode(lag,"lag", 0,(yyvsp[(3) - (4)])); }
    break;

  case 100:
#line 258 "parse.y"
    { (yyval) = newnode(led,"lead",0,(yyvsp[(3) - (4)])); }
    break;

  case 101:
#line 259 "parse.y"
    { (yyval) = newnode(led,"lead",0,(yyvsp[(3) - (4)])); }
    break;

  case 102:
#line 260 "parse.y"
    { (yyval) = newnode(led,"lead",0,(yyvsp[(3) - (4)])); }
    break;

  case 103:
#line 261 "parse.y"
    { (yyval) = newnode(led,"lead",0,(yyvsp[(3) - (4)])); }
    break;

  case 104:
#line 264 "parse.y"
    { (yyval) = newnode(sum,"sum"  ,(yyvsp[(3) - (6)]),(yyvsp[(5) - (6)])); }
    break;

  case 105:
#line 265 "parse.y"
    { (yyval) = newnode(prd,"prod" ,(yyvsp[(3) - (6)]),(yyvsp[(5) - (6)])); }
    break;


/* Line 1267 of yacc.c.  */
#line 2037 "parse.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


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

