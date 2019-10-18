/*--------------------------------------------------------------------*
 *  LEXICAL.C 1.00
 *  9 Apr 90 (PJW)
 *
 *  Lexical analysis routine for symbolic math program.
 * 
 *  Should be included at the end of the parser (yacc) specification
 *  so that it can use the same token names.
 *
 *  Only integers are acceptable as numbers.
 *--------------------------------------------------------------------*/

#include "lexical.h"

#include "error.h"
#include "nodes.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKBUF 132

static void unput();

int n_stmt;
int n_line;

enum lex_acts { rtn_only, rtn_node, null };

struct { 
   char *word;
   enum lex_acts act;
   int rtn;
   } 
   reserved_list[]=
   {
   "equation",	 rtn_only,	EQU,
   "exp",	    rtn_only,	EXP,
   "first",     rtn_node,  FIRST,
   "lag",	    rtn_only,	LAG,
   "last",      rtn_node,  LAST,
   "lead",	    rtn_only,	LEAD,
   "ln",	       rtn_only,	LOG,
   "log",	    rtn_only,	LOG,
   "next",	    rtn_only,	NEXT,
   "par",       rtn_node,	PAR,
   "parameter", rtn_node,	PAR,
   "prod",      rtn_only,  PROD,
   "product",   rtn_only,  PROD,
   "set",       rtn_node,	SET,
   "sum",       rtn_only,  SUM,
   "time",      rtn_node,  TIME,
   "union",     rtn_node,  UNION,
   "var",	    rtn_node,	VAR,
   "variable",	 rtn_node,	VAR,
   0,		       rtn_node,	NAME
   };


int yylex()
{
   static char token[TOKBUF];
   char cur,input();
   int i;

   for( cur=input() ; isspace(cur) ; cur=input() );
   if( cur=='\0' )return cur;

   if( cur=='\'' )
      {
      i=0;
      for( cur=input() ; cur && cur!='\'' ; cur=input() ) {
         if( i == TOKBUF-2 ) {
            token[i++] = '\0';
            fatal_error("token longer than buffer: %s...",token);
         }
         token[i++] = cur;
      }
      token[i] = '\0';
      yylval = newnode(nam,token,0,0);
      return STRING;
      }

   if( cur=='*' )
      {
      if( (cur=input())=='*' )
         return '^';
      else
         unput(cur);
      return '*';
      }

   switch( cur )
      {
      case '+':
      case '-':
      case '/':
      case '^':
      case '=':
      case '(':
      case ')':
      case ',':
      case '#':
      case ':':
      case ';':
      case '[':
      case ']':
      case '{':
      case '}':
         return cur;
      }
      
   if( isdigit(cur) || cur=='.' )
      {
      for( i=0 ; isdigit(cur) || cur=='.' ; cur=input() )token[i++] = cur;
      token[i] = '\0';
      unput(cur);
      if( strchr(token,'.') != strrchr(token,'.') )
         return BADNUM;
      yylval = newnode(num,token,0,0);
      return NUM;
      }

   if( !isalnum(cur) )
      {
      sprintf(token,"Inappropriate character in file: '%c' (octal code 0%o)",cur,(int) cur);
      fatal_error("%s",token);
      }

   for( i=0 ; isalnum(cur) || cur == '_' ; cur=input() )token[i++] = cur;
   token[i] = '\0';
   unput(cur);

   for( i=0 ; reserved_list[i].word != 0 ; i++ )
      if( isequal(token,reserved_list[i].word) )break;

   switch( reserved_list[i].act )
      {
      case rtn_only: return reserved_list[i].rtn;
      case rtn_node: yylval = newnode(nam,token,0,0); break;
      default: fatal_error("%s","undefined state in yylex()");
      }

   return reserved_list[i].rtn;
}

char *lex_buf=0;
int next_lex;

void load_lex_buf(str)
char *str;
{
   if( lex_buf )
      free( lex_buf );

   lex_buf = strdup(str);
   next_lex = 0;
}


char input()
{
   if( lex_buf[ next_lex ] == 0 )return 0;
   return lex_buf[ next_lex++ ];
}

static void unput(c)
char c;
{
   if( next_lex <= 0 )fatal_error("%s","could not unput()");
   lex_buf[ --next_lex ] = c;
}

char *getlexbuf()
{
   return lex_buf;
}
