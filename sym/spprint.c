/*-------------------------------------------------------------------*
 *  SPPRINT.C 2.25
 *  20 Mar 90 (PJW)
 *
 *  Print a node into a character buffer following precedence rules
 *-------------------------------------------------------------------*/

#include "spprint.h"

#include "error.h"
#include "nodes.h"
#include "str.h"
#include "sym.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define now(arg) (cur->type == arg)

static char *spprint();


/*-------------------------------------------------------------------*
 *  node2string
 * 
 *  Return a new string corresponding to the node.  Don't do any
 *  fancy indenting or line breaking.
 *-------------------------------------------------------------------*/
char *node2string(Node *node)
{
   return spprint(nul,node,0);
}


/*-------------------------------------------------------------------*
 *  snprint
 *
 *  Streamlined interface; returns a newly-allocated string.
 *-------------------------------------------------------------------*/
char *snprint(Node *node)
{
   return spprint(nul,node,"   ");
}


/*-------------------------------------------------------------------*
 *  sniprint
 *
 *  Selectable indenting.
 *-------------------------------------------------------------------*/
char *sniprint(Node *node,char *indent)
{
   return spprint(nul,node,indent);
}


/*-------------------------------------------------------------------*
 *  spprint
 *
 *  Do the actual work
 *-------------------------------------------------------------------*/
static char *spprint(Nodetype prevtype, Node *cur, char *indent)
{
   int parens,wrap_right;
   char *comma,*cr;
   char *lstr,*rstr,*chunk;

   if( cur==0 )return strdup("");

   parens=0;
   comma ="";
   switch( prevtype )
      {
      case nul:
      case add:
      case sub:
         if( now(neg) )parens=1;
         break;

      case mul:
         if( now(add) || now(sub) )parens=1;
         if( now(dvd) || now(neg) )parens=1;
         break;

      case neg:
         parens=1;
         if( now(nam) || now(num) || now(mul) )parens=0;
         if( now(log) || now(exp) || now(pow) )parens=0;
         if( now(sum) || now(prd)             )parens=0;
         break;

      case dvd:
         parens=1;
         if( now(nam) || now(num) || now(pow) )parens=0;
         if( now(sum) || now(prd)             )parens=0;
         if( now(log) || now(exp)             )parens=0;
         break;

      case pow:
         parens=1;
         if( now(nam) || now(num) || now(log) || now(exp) )parens=0;
         if( now(sum) || now(prd)                         )parens=0;
         break;

      case log:
      case exp:
      case lag:
      case led:
         parens = 1;
         break;

      case equ:
      case sum:
      case prd:
      case dom:
         break;

      case nam:
      case num:
         if( now(nam) || now(num) )comma = ",";
         break;

      default:
         fatal_error("%s","invalid state reached in spprint");
      }
   
   switch( cur->type )
      {
      case sum:
      case prd:
         lstr = spprint( cur->type , cur->l, indent );
         rstr = spprint( cur->type , cur->r, indent );
         chunk = concat(6,cur->str,"(",lstr,",",rstr,")");
         free(lstr);
         free(rstr);
         break;

      case lst:
         lstr = strdup("(");
         for( cur=cur->r ;  cur ; cur=cur->r ) {
            if( cur->r )
               rstr = concat(3,lstr,cur->str,",");
            else
               rstr = concat(2,lstr,cur->str);
            free(lstr);
            lstr = rstr;
         }
         chunk = concat(2,lstr,")");
         free(lstr);
         break;

      default:
         lstr = spprint( cur->type , cur->l, indent );
         rstr = spprint( cur->type , cur->r, indent );

         cr = "" ;
         if( indent )
            if( strlen(lstr)+strlen(rstr) > 70 
                || strlen(lstr) > 40
                || strlen(rstr) > 40 )cr = concat(2,"\n",indent);

         wrap_right = 0;
         if( cur->type == sub ) 
            if( cur->r->type == add || cur->r->type == sub )
               wrap_right = 1;

         if( parens )
            if( wrap_right) 
               chunk = concat(8,"(",lstr,comma,cr,cur->str,"(",rstr,"))");
            else
               chunk = concat(7,"(",lstr,comma,cr,cur->str,rstr,")");
         else
            if( wrap_right )
               chunk = concat(7,lstr,comma,cr,cur->str,"(",rstr,")");
            else
               chunk = concat(5,lstr,comma,cr,cur->str,rstr);

         if( strlen(cr) )free( cr );
         free(lstr);
         free(rstr);
      }

   return chunk;
}


