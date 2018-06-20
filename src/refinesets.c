/*-------------------------------------------------------------------*
 * refinesets(master,subsets,strong)
 *
 *  Impose a list of restrictions on a list of sets.
 *
 *  The 'strong' parameter controls what happens if a restriction
 *  can't be applied because the variable is not defined over the
 *  set appearing in the restriction.
 *  
 *     strong == 0 => Ignore the restriction.  This is the
 *                    correct setting when applying equation-wide
 *                    qualifiers.  
 * 
 *     strong != 0 => Abort the program with an error message.
 *                    Use this setting when applying explicit
 *                    restrictions attached to individual 
 *                    variable names.
 *  
 *  Examples:
 *
 *     producers: rev = tax*(p*q-w*L) ;
 *
 *        Use strong==0 to keep refinesets from insisting that 
 *        tax and wage be subscripted by 'producers'
 *        
 *     klem(varfac) = x*y*z(varfac);
 *
 *        Use strong==1 to make sure that 'varfac' is a
 *        legitimate restriction on klem and z.
 *
 *-------------------------------------------------------------------*/

#include "error.h"
#include "lists.h"
#include "sets.h"
#include "str.h"
#include "sym.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 0

List *refinesets(master,subsets,strong)
List *master,*subsets;
int strong;
{
   List *res,*todo,*ex;
   Item *m,*s,*e;
   int hit,exact,nchanges;
   
   validate( master,  LISTOBJ, "refinesets (master)"  );
   validate( subsets, LISTOBJ, "refinesets (subsets)" );

   res  = newsequence();

   if( subsets->n == 0 )
      {
      catlist( res, master );
      return res;
      }
      
   ex   = newsequence();
   todo = newsequence();
   catlist( todo, subsets );
   
   //  check for ambiguities by counting the number of potential
   //  matches for each set in the master list

   for( m=master->first ; m ; m=m->next )
      {
      hit = 0;
      exact = 0;
      for( s=subsets->first ; s ; s=s->next )
         {
         if( isequal(s->str,m->str) )
            {
            exact++;
            continue;
            }
         if( issubset(s->str,m->str) )
            {
            hit++;
            continue;
            }
         }
      if( exact == 1 )
         {
         addlist( ex, m->str );
         freeitem(todo,m->str);
         continue;
         }
      addlist( ex, "" );
      if( exact == 0 && hit <= 1 )continue;
      
      error_front("Set '%s' restricted by multiple items in (%s)\n",m->str,slprint(subsets));
      exit(0);
      }

   //  now go the other way: make sure that each restriction matches at 
   //  most one element of the master list

   for( s=subsets->first ; s ; s=s->next )
      {
      hit = 0;
      exact = 0;
      for( m=master->first ; m ; m=m->next )
         {
         if( isequal(s->str,m->str) )
            {
            exact++;
            continue;
            }
         if( issubset(s->str,m->str) )
            {
            hit++;
            continue;
            }
         }
      if( exact == 1 )continue;
      if( exact == 0 && hit == 1 )continue;
      if( exact == 0 && hit == 0 )
         {
         if( strong == 0 )continue;
         error_front("Set restriction '%s' cannot be applied to any elements of (%s)\n",s->str,slprint(master));
         exit(0);
         }
      error_front("Restriction '%s' matches multiple items in (%s)\n",s->str,slprint(master));
      exit(0);
      }

   //  ok, we should be good to go from here...

   nchanges = 0;
   for( m=master->first, e=ex->first ; m ; m=m->next, e=e->next )
      {
      if( strlen(e->str) > 0 )
         {
         addlist(res,e->str);
         nchanges++;
         continue;
         }

      hit = 0;
      for( s=todo->first ; s ; s=s->next )
         {
         if( isequal(s->str,m->str) )continue;
         if( issubset(s->str,m->str) )
            {
            addlist(res,s->str);
            nchanges++;
            hit = 1;
            break;
            }
         }
      if( hit )continue;

      //  no match for this set in the list of refinements; pass
      //  it along without modification.

      addlist(res,m->str);
      }

   freelist( todo );
   freelist( ex );

   if( DBG )
      {
      printf("   ... refined [%s] ",slprint(master));
      printf("using [%s] ",slprint(subsets));
      printf("-> [%s] ",slprint(res));
      printf("changes=%d expected=%d\n",nchanges,subsets->n);
      }

   return res;
}

